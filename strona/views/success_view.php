
    <?php include "fragments/layout/head.php"; ?>

      

       <div id="midd">
        <div id="left-side">
        <div class="article">
            <div class="artphoto"><img src="static/img/info.png" alt="John Terry"/></div>
            <div class="articletext">
                <div class="arttitle"><p>Komunikat</p></div>
                <div class="artdesc">
                    Witaj <?= $mode['nick'] ?>!<?= $mode['tryb']=="logowanie"?" Zostales zalogowany" :" Rejestracja przebiegla pomyslnie. Mozesz sie teraz zalogowac."?>
                </div>
            </div>
        </div>


        </div>

           <?php include "fragments/layout/right-side.php"?>
           </div>


<?php include "fragments/layout/footer.php"; ?>

</body>
</html>
