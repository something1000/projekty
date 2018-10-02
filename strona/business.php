<?php
$upload_dir = '/var/www/dev/web/images/';

function get_db()
{
    $mongo = new MongoClient(
        "mongodb://localhost:27017/",
        [
            'username' => 'wai_web',
            'password' => 'w@i_w3b',
            'db' => 'wai',
        ]);

    $db = $mongo->wai;

    return $db;
}




function loginUser($user,&$model){
    $db = get_db();

    $my_user = $db->userzy->findOne(['login' => $user['login']]);
    if($user !== null &&
        password_verify($user['password'], $my_user['password'])){
        session_regenerate_id();
        $_SESSION['user_id'] = $my_user['_id'];
        $model['mode'] = [
            "nick" => $_POST['login_'],
            "tryb" => "logowanie"];
        $_SESSION['logged'] =1;
        $_SESSION['nick'] = $_POST['login_'];
        return 1;
    }
}

function getImages(){
    $db = get_db();

    $images = $db->images->find();
    $images = deletePrivates($images);
    return $images;
}

function deletePrivates($images)
{
    $newImages = [];
    echo empty($_SESSION['nick']);

    if (empty($_SESSION['nick'])) {

        foreach ($images as $item) {
            if ($item['private'] == '1') {
                echo 'mam';
            } else {

                echo 'niemam ';
                $newImages[] = $item;
            }
        }
    } else {
        foreach ($images as $item) {
            if ($item['private'] == '1' && $_SESSION['nick'] == $item['author']) {
                $newImages[] = $item;
            } else if(!($item['private'] == '1')){
                echo 'niemam ';
                $newImages[] = $item;
            }
        }

    }
    return $newImages;
}

function addImageToDb($info){
    $db = get_db();

    $db->images->insert($info);
}

function addWatermarkImg($image,$watermark,$name){
    global $upload_dir;
    $file_name = $name;

    if($image['type']=='image/png'){
        $im = imagecreatefrompng ( $upload_dir.$file_name.'.png' );
        $file_name = basename($file_name,'.png');
        $file_name = basename($file_name,'.png');
    } else {
        $im = imagecreatefromjpeg( $upload_dir.$file_name.'.jpg' );
        $file_name = basename($file_name,'.jpg');
        $file_name = basename($file_name,'.JPG');
    }

    $im_width = imagesx($im);
    $im_height = imagesy($im);
    $wm_pos_x =$im_width - ($im_width*0.3);
    $wm_pos_y =$im_height - ($im_height*0.05);
    $font_size=$im_width*0.05;

    $color = imagecolorallocate($im,24,94,164);
    imagefttext($im,$font_size,0,$wm_pos_x,$wm_pos_y,$color,'./font.ttf',$watermark);
    imagejpeg($im,$upload_dir.$file_name.'_wm'.'.png');
}

function onlySaved($allImg){

    $i =0;
    $images=[];
    foreach($allImg as $item){


        if(!empty($_SESSION['remember'][(string)$item['_id']])){
            $images[] = $item;
            echo $i;
        }
        $i++;
    }
    return $images;
}

function forgetSaved($remembered){
    foreach($remembered as $key => $item){
        $_SESSION['remember'][$key] = $item==1?0:1;
    }
}

function rememberGallery($toremember){
    $_SESSION['remember'] = $toremember;
}


function addMiniatureImg($image,$im_name){
    global $upload_dir;
    $file_name = basename($image['name']);

    if($image['type']=='image/png'){
        $im = imagecreatefrompng ( $upload_dir.$im_name.'.png' );
        $im_name = basename($im_name,'.png');
        $im_name = basename($im_name,'.png');
    } else {
        $im = imagecreatefromjpeg( $upload_dir.$im_name.'.jpg' );
        $im_name = basename($im_name,'.jpg');
        $im_name = basename($im_name,'.JPG');
    }
    $width = imagesx($im);
    $height = imagesy($im);
    $miniature = imagecreatetruecolor(200,125);
    imagecopyresized($miniature,$im,0,0,0,0,200,125,$width,$height);
    imagejpeg($miniature,$upload_dir.$im_name.'_min.png');
}

function uploadImage($image,$name){
   global $upload_dir;
    if($image['type']==='image/png'){
        $name = $name.'.png';
    } else {
        $name = $name.'.jpg';
    }

    $file_name = basename($image['name']);
    $target = $upload_dir . $name;
    $tmp_path = $image['tmp_name'];
    if(move_uploaded_file($tmp_path, $target)){
        return 1;
    }
    return 0;
}

function validateFileExt($image){
    $finfo = finfo_open(FILEINFO_MIME_TYPE);
    $file_name = $image['tmp_name'];
    $mime_type = finfo_file($finfo, $file_name);
    if ($mime_type === 'image/jpeg' || $mime_type === 'image/png') {
        return 0;
    } else {
        return 1;
    }
}

function validateFileSize($image){
    if($image['size'] > 1000000){
        return 1;
    } return 0;
}


function registerUser($new_user){
    $db = get_db();

    $hash = password_hash($new_user['password'],PASSWORD_DEFAULT);
    $new_user['password'] = $hash;
    $db->userzy->insert($new_user);
}

function validateUser($login,$mail,$pass,$pass_re){
    $db = get_db();

    foreach($db->userzy->find() as $user){

    if($login == $user['login']){
        return 1;
    }else if($mail == $user['email']){
        return 2;
    }
    }
    if($pass != $pass_re){
        return 3;
    }
    return 0;
}