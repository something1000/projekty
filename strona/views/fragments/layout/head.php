

    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <link rel="stylesheet" type="text/css" href="static/css/style.css" />
        <title>Chelsea F.C Hipertekst i hipermedia</title>
        <script src="static/js/scripts.js"></script>
        <script src="static/js/jquery.js"></script>
        <script src="static/js/slider_oper.js"></script>
        <script src="static/js/jquery-1.11.3.min.js" type="text/javascript"></script>
        <script src="static/js/jquery-ui-1.12.1/jquery-ui.js"></script>
    </head>
    <body onload="index_load()">

    <div id="kontener" >
        <div id="top">

            <div id="header">
                <div id="left-head">
                    <div id="logoplace">
                        <img class="cfc" src="static/img/chelsea.png" alt="chelsea"/>
                        <script>

                        </script>
                        <span class="logotxt">Chelsea FC</span><br />
                        <span class="desctxt">Keep the blue flag flying high</span> <br />
                        <span class="projtxt">Hypertext and hypermedia project</span><br />
                    </div></div>
                <div id="noJS">Masz wyłączoną obsługe JavaScript. Niektóre elementy strony mogą nie działać.</div>
            </div>

            <div id="menu">
                <ol class="menu-items">
                    <li class="menu-el"><span class="mtxtup"><a href="home">Home</a></span></li>
                    <li class="menu-el"><span class="mtxtup"><a href="gallery">Galeria</a></span></li>
                    <li class="menu-el"><span class="mtxtup">Skład</span><ul><li><a href="kadra.html">Kadra</a></li><li><a onclick="wbudowie()" href="#">Transfery</a></li></ul></li>
                    <li class="menu-el"><span class="mtxtup"><a href="history.html">Historia</a></span></li>
                    <li class="menu-el"><span class="mtxtup"><a href="contact.html">Kontakt</a></span></li>
                    <li class="menu-el"><span class="mtxtup"><?=empty($_SESSION["logged"])?'<a href="login">Logowanie</a>':'<a href="logout">Wyloguj</a>'?></span></li>
                </ol>
            </div>
        </div>
