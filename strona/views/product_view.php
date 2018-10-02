<!DOCTYPE html>
<html>
<head>
    <title>Produkt</title>
    <?php include "fragments/layout/head.php"; ?>
</head>
<body>

<h1><?= $product['name'] ?></h1>

<p>Cena: <?= $product['price'] ?> PLN</p>

<p class="description"><?= $product['description'] ?></p>

<form action="cart/add" method="post" class="wide" data-role="cart_form">
    <input type="hidden" name="id" value="<?= $product['_id'] ?>"/>

    <div>
        <a href="products" class="cancel">&laquo; Wróć</a>
        <input type="submit" name="add_to_cart" value="Do koszyka"/>
    </div>
</form>

<div id="cart_container">
    <?php dispatch($routing, '/cart') ?>
</div>

<?php include "fragments/layout/footer.php"; ?>

</body>
</html>
