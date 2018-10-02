
    <?php include_once "fragments/layout/head.php"; ?>

       <div id="midd">
           <div id="left-side" >
               <div class="norfield">
                   <div class="norfieldtext">
                       <div class="norfieldtitle"><p>Galeria || <a href="add">Dodaj</a> || <a href="remembered">Zapamietane</a></p> </div>
                       <div class="norfieldcnt" >
                            <p style="color:red">
                                <?=
                                empty($upload_status)?' ':'Plik zostal dodany!';
                                ?>
                            </p>

                           <div id="gallery">
                               <?php if (!empty($images)) :?>
                                   <form action="" method="post">

                                    <?php foreach($images as $img): ?>
                                       <div class="g_image_contener">
                                           <div class="g_img"><a href="<?= 'images/'.$img['name'].'_wm.png'?>" target="_blank"><img src="<?= 'images/'.$img['name'].'_min.png'?>" alt="<?= $img['name']?>" /></a></div>
                                           <div class="g_title">
                                               <table>
                                                   <tr>
                                                       <td>Tytul: </td>
                                                       <td><?= $img['name']?></td>
                                                   </tr>
                                                   <tr>
                                                       <td>Autor: </td>
                                                       <td> <?=
                                                           $img['author'].
                                                           (!empty($_SESSION['nick'])?(($_SESSION['nick']==$img['author'])&& $img['private']==1?(' (priv.)'):('')):(''))
                                                           ?>

                                                       </td>
                                                   </tr>
                                                   <tr>
                                                       <td>Zaznacz:</td>
                                                       <td>
                                                           <input type="checkbox" name="remember[<?=$img['_id']?>]" value="1" <?=  empty($_SESSION['remember'][(string)$img['_id']])? ' ':'checked' ?>/>


                                                       </td>
                                                   </tr>
                                               </table></div>
                                       </div>
                                    <?php endforeach ?><br/>

                                       <div style="float:left;width:100%; text-align: center    "><input type="submit" value="Zapamietaj zaznaczone"/></div>
                                   </form>

                            <?php endif ?>
                           </div>

                       </div>
                   </div>
               </div>
           </div>
           <?php include "fragments/layout/right-side.php"?>
           <?php include_once "fragments/layout/footer.php"; ?>
           </div>





</body>
</html>
