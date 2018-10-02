
    <?php include "fragments/layout/head.php"; ?>

       <div id="midd">
           <div id="left-side" >
               <div class="norfield">
                   <div class="norfieldtext">
                       <div class="norfieldtitle"><p>Dodawanie</p> </div>
                       <div class="norfieldcnt" >
                            <p style="color:red">
                                <?=
                                empty($upload_err['size'])?' ':'Za duzy rozmiar pliku</br>';
                                ?>
                                <?=
                                empty($upload_err['extension'])?' ':'Zle rozszerzenie pliku !';
                                ?>
                            </p>
                           <form action="add" method="post" enctype="multipart/form-data">
                           <table style="margin:0 auto">
                               <tr>
                                   <td></td>
                                   <td><input type="file" name="image"/></td>
                               </tr>

                               <tr>
                                   <td>Nazwa:</td>
                                   <td><input type="text" name="img_name"/></td>
                               </tr>

                               <tr>
                                   <td>Autor: </td>
                                   <td><input type="text" name="author" value="<?= empty($_SESSION['nick'])?'':$_SESSION['nick'] ?>"/></td>
                               </tr>
                               <tr>
                                   <td>Znak wodny: </td>
                                   <td><input type="text" name="watermark"/></td>
                               </tr>
                               <?= empty($_SESSION['logged'])?'':'
                             <tr>
                                   <td>Prywatne: </td>
                                   <td><input name="private" type="radio"  value="1"/></td>
                               </tr>
                                <tr>
                                   <td>Publiczne: </td>
                                   <td><input name="private" type="radio" value="0" checked="true"/></td>
                               </tr>
                               '?>

                               <tr>
                                   <td><input type="reset" value="Reset"/></td>
                                   <td><input type="submit" value="Dodaj"/></td>
                               </tr>
                           </table>
                           </form>

                       </div>
                   </div>
               </div>
           </div>
        <?php include "fragments/layout/right-side.php"?>

           <?php include "fragments/layout/footer.php"; ?>
           </div>





</body>
</html>
