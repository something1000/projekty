
    <?php include "fragments/layout/head.php"; ?>



   <div id="midd">
       <div id="left-side" >
           <div class="norfield">
               <div class="norfieldtext">
                   <div class="norfieldtitle"><p>Logowanie</p></div>
                   <div class="norfieldcnt" >
                       <br /><br />
                       <form id="register_form" method="post" action="register" >
                           <p name="komunikat" style="color:red">

                            <?= $reg_err ==1?'Login zajety': ''?>
                            <?= $reg_err ==2?'Wybrany mail zostal juz zarejestrowany': ''?>
                            <?= $reg_err ==3?'Hasla nie sa takie same': ''?>

                           </p>
                           <table id='login_form_tb' style='margin: 0 auto;'>
                               <tr>
                                   <td><label>Login:</label></td>
                                   <td><input type='text' name='login' onkeyup="chLogin(this.value)"value="<?= isset($_POST['login'])?$_POST['login'] : null ?>" required/></td>
                                   <td class="badlogin"> &nbsp;&nbsp;</td>
                               </tr>
                               <tr>
                                   <td><label>E-mail:</label></td>
                                   <td><input type='text' name='email'onkeyup="chMail(this.value)" value="<?= isset($_POST['email_'])?$_POST['email_'] : null ?>" required/></td>
                                   <td class="badlogin"> &nbsp;&nbsp;</td>
                               </tr>
                               <tr>
                                   <td><label>Haslo:</label></td>
                                   <td><input type='password' name='password' onkeyup="chPass(this.value,0)" required/></td>
                                   <td class="badlogin"> &nbsp;&nbsp;</td>
                               </tr>
                               <tr>
                                   <td><label>Powtorz Haslo:</label></td>
                                   <td><input type='password' name='password_pow' onkeyup="chPass(this.value,1)" required></td>
                                   <td class="badlogin"> &nbsp;&nbsp;</td>
                               </tr>
                               <tr>
                                   <td></td>
                                   <td> <input type='submit' value='Zarejestruj'/>&nbsp;<input type='reset' value='Reset'/></td>
                               </tr>
                               <input type="hidden" name="err_code" value="<?= isset($_POST['reg_err'])?$_POST['reg_err'] : 0 ?>"/>
                           </table>
                       </form>
                   </div>
               </div>
           </div>
       </div>
        <?php include "fragments/layout/right-side.php" ?>
           </div>

       <?php include "fragments/layout/footer.php"; ?>

    </div>




</body>
</html>
