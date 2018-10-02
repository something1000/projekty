
    <?php include "fragments/layout/head.php"; ?>



       <div id="midd">
           <div id="left-side" >
               <div class="norfield">
                   <div class="norfieldtext">
                       <div class="norfieldtitle"><p>Logowanie</p></div>
                       <div class="norfieldcnt" >
                           <br /><br />
                           <p style="color:red;" class="err_log">
                               <?=
                               (!empty($badlogin))? 'Nieprawidlowe haslo lub nazwa uzytkownika' : '';
                               ?>
                               <?=
                               (!empty($_SESSION['successReg']))? 'Pomyślnie się zarejestrowałeś <b>'.$_SESSION['successReg'].'</b>': '';?>
                               <?php unset($_SESSION['successReg'])?>
                           </p>
                           <form id="login_form" method="post" action="login" >
                               <table id="login_form_tb" style="margin: 0 auto;">
                                   <tr>
                                       <td><label>Login:</label></td>
                                       <td><input type="text" name="login_" required/></td>
                                   </tr>
                                   <tr>
                                       <td><label>Haslo:</label></td>
                                       <td><input type="password" name="haslo_" required/></td>
                                   </tr>
                                   <tr>
                                       <td></td>
                                      <td> <input type="submit" value="Login"/>&nbsp;<input type="reset" value="Reset"/></td>
                                   </tr>
                                   <tr>
                                       <td></td>
                                       <td> Nie masz konta? <a href="register"><strong>Zarejestruj sie!<strong></a></td>
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
