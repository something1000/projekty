
var switched = false;
var data;


/*function changeAction(){
    $("#login_form").attr("action","register");
    $("#login_form_tb").remove();
    $("#login_form").append("\
     <table id='login_form_tb' style='margin: 0 auto;'> \
     <tr>\
         <td><label>Login:</label></td>\
         <td><input type='text' name='login_' onkeyup='checkLogin(this.value)'/></td>\
     </tr>\
     <tr>\
        <td><label>E-mail:</label></td>\
        <td><input type='text' name='email_' /></td>\
     </tr>\
     <tr>\
        <td><label>Haslo:</label></td>\
        <td><input type='password' name='haslo_' /></td>\
     </tr>\
     <tr>\
        <td><label>Powtorz Haslo:</label></td>\
        <td><input type='password' name='haslo_repeat'></td>\
     </tr>\
     <tr>\
        <td></td>\
        <td> <input type='submit' value='Login'/>&nbsp;<input type='reset' value='Reset'/></td>\
     </tr>\
    </table>\
    ");

}
*/


function chLogin(login){
    if(login.length <3){
    document.getElementsByClassName("badlogin")[0].innerHTML = "&nbsp;X";
    } else {
        document.getElementsByClassName("badlogin")[0].innerHTML = "OK";
    }
}

function chMail(mail){
    var validator= new RegExp("[a-zA-Z0-9]+[@][a-zA-Z0-9]+\.[a-zA-Z]+");

    if(!validator.test(mail)){
        document.getElementsByClassName("badlogin")[1].innerHTML = "&nbsp;X";
    } else {
        document.getElementsByClassName("badlogin")[1].innerHTML = "OK";
    }
}

function chPass(pass, pos){
    if(pass.length <6){
        document.getElementsByClassName("badlogin")[2+pos].innerHTML = "&nbsp;X";
    } else {
        document.getElementsByClassName("badlogin")[2+pos].innerHTML = "OK";
    }
}

function form_check(){

    var error = document.getElementsByName('err_code')[0].value;
    if(error==0){
        return;
    } else if(error==1){
        document.getElementsByName("komunikat")[0].innerHTML = "Wybrany login jest zajety!";
    }
    else if(error==2){
        document.getElementsByName("komunikat")[0].innerHTML = "Wybrany mail jest zarejestrowany!";
    } else if(error==3){
        document.getElementsByName("komunikat")[0].innerHTML = "Hasla nie sa takie same!";
    }
    else if(error==4){
        document.getElementsByName("komunikat")[0].innerHTML = "dziala?";
    }
}

function index_load() {
    document.getElementById("header").removeChild(document.getElementById("noJS"));
}


function switchingMatch(czas) {
    if (switched == false) {
        $(".nextTeams").slideUp(czas);
        setTimeout(function () {
            data = document.getElementsByClassName("timer")[0].innerHTML;
            document.getElementsByClassName("home")[0].src = 'img/oponent_next.png';
            document.getElementsByClassName("away")[0].src = 'img/chelsea.png';
            document.getElementsByClassName("nextTitle")[0].innerHTML = "<span>Poprzedni mecz</span>";
            document.getElementsByClassName("timer")[0].innerHTML = "10-10-2016 || 14:00<br />Manchester United 1 vs. 3 Chelsea Londyn F.C";
            document.getElementsByClassName("switchmatch")[0].innerHTML = "Nastepny mecz &gt;&gt;";
            document.getElementsByClassName("relacja")[0].style.visibility = "hidden";
            document.getElementsByClassName("switchmatch")[0].style.float = "right";
            switched = true;
        }, czas);

        $(".nextTeams").slideDown(czas);
    }
    else {
        $(".nextTeams").slideUp(czas);
        setTimeout(function () {
            document.getElementsByClassName("home")[0].src = 'img/chelsea.png';
            document.getElementsByClassName("away")[0].src = 'img/oponent_last.png';
            document.getElementsByClassName("timer")[0].innerHTML = data;
            document.getElementsByClassName("nextTitle")[0].innerHTML = "<span>Nastepny mecz</span>";
            document.getElementsByClassName("switchmatch")[0].innerHTML = "&lt;&lt; Poprzedni mecz";
            document.getElementsByClassName("relacja")[0].style.visibility = "visible";
            document.getElementsByClassName("switchmatch")[0].style.float = "left";
            switched = false;
        }, czas)
        $(".nextTeams").slideDown(czas);
    }

}
var clicked =false;
function wbudowie() {
    if (clicked == false) {
        var a = document.createElement("div");
        a.setAttribute("id", "maintain");
        var x = document.createTextNode("Wybrana zakladka jest w budowie. Przepraszamy bardzo za utrudnienia.");
        a.appendChild(x);
        a.style.float = "left";
        a.style.width = "100%";
        a.style.backgroundColor = "#ff0000";
        document.getElementById("top").appendChild(a);
        setTimeout("destroy()", 4000);
        clicked = true;
    }
}

function destroy() {
    var asd = document.getElementById("top");
    asd.removeChild(document.getElementById("maintain"));
    clicked = false;
}


function saveStorage() {
    localStorage.imie = document.getElementsByName("imie")[0].value;
    localStorage.nazwisko = document.getElementsByName("nazwisko")[0].value;
    localStorage.email = document.getElementsByName("e-mail")[0].value;

}
function loadStorage() {
    if (localStorage.length !=0) {
        document.getElementsByName("imie")[0].value = localStorage.imie;
        document.getElementsByName("nazwisko")[0].value = localStorage.nazwisko;
        document.getElementsByName("e-mail")[0].value = localStorage.email;
    }
}

