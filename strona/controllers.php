<?php
require_once 'business.php';



function home(&$model)
{
    return 'home_view';
}

function register(&$model)
{
    $user = [
        'login' => null,
        'password' => null,
        'email' => null
    ];
    $model['reg_err'] = 0;
    if ($_SERVER['REQUEST_METHOD'] === 'POST') {

        $model['reg_err'] = validateUser($_POST['login'],$_POST['email'],$_POST['password'],$_POST['password_pow']); // 1 zly login,
                if(((int)$model['reg_err'])) {
                    return 'register_view';
                } else {
                    $user['login'] = $_POST['login'];
                    $user['password'] = $_POST['password'];
                    $user['email'] = $_POST['email'];
                }
        registerUser($user);
        $model['mode'] = [
            "nick" => $_POST['login'],
            "tryb" => "rejestracja"];

        $_SESSION['successReg'] = $_POST['login'];
        return 'redirect:login';
    }
    return 'register_view';
}


function logout(){
    if(empty($_SESSION['logged'])){
    header('Location: /home');
} else {
        $params = session_get_cookie_params();
        setcookie(session_name(),
            ''
            , time() - 42000,
            $params["path"], $params["domain"],
            $params["secure"], $params["httponly"]
        );
        session_destroy();
    }
    header('Location: /home');
    exit();
}

function gallery_add(&$model)
{
    if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
        return 'gallery_add_view';
    } else {

        $uploaded_image = $_FILES['image'];

        $format_status = validateFileExt($uploaded_image);
        $size_status = validateFileSize($uploaded_image);

        $model['upload_err'] = [
            "extension" => $format_status,
            "size" => $size_status];

        if ($format_status == 1 || $size_status == 1) {

            return 'gallery_add_view';
        }

        $im_name = $_POST['img_name'];


        uploadImage($uploaded_image, $im_name);
        addWatermarkImg($uploaded_image, $_POST['watermark'], $im_name);
        addMiniatureImg($uploaded_image, $im_name);

        $image_db_info = [
            'name' => $_POST['img_name'],
            'author' => $_POST['author'],
            'private' => empty($_POST['private'])?0:$_POST['private'],
            'type' => $uploaded_image['type']==='image/png'?'png':'jpg'
        ];

        addImageToDB($image_db_info);

        $model['upload_status'] = 1;
        return 'redirect:gallery';
    }

}
function gallery(&$model){

    if($_SERVER['REQUEST_METHOD'] === 'POST'){
       rememberGallery($_POST['remember']);
    }
    $images = getImages();

    $model['images']=$images;

    return 'gallery_view';
}



function remembered(&$model){
    if($_SERVER['REQUEST_METHOD'] === 'POST'){
        forgetSaved($_SESSION['toforget'] = $_POST['toforget']);
    }
    $images=getImages();
    $savedImg = onlySaved($images);
    $model['saved'] = $savedImg;
    return 'remembered_view';
}

function login(&$model)
{
    if(!empty($_SESSION['logged']) && $_SESSION['logged'] ==1){
        header('Location: /home');
    }

    if(empty($_POST['login_'])|| $_SERVER['REQUEST_METHOD'] !== 'POST'){
        return 'login_view';
    }
    $user = [
        'password' => $_POST['haslo_'],
        'login' => $_POST['login_']
    ];


    if(loginUser($user,$model)){
        return 'redirect:success';
    } else {
        $model['badlogin'] = 1;
    }

    return 'login_view';
}

function success(&$model){
    if(empty($model['mode']['nick'])){
        header('Location: home');
        exit();
    }
    return 'success_view';
}



