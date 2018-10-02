<!DOCTYPE html>
<html>
<head>
    <title>Usuwanie produktu</title>
    <?php include "fragments/layout/head.php"; ?>
</head>
<body>

<form method="post">
    Czy usunąć produkt: <?= $product['name'] ?>?

    <input type="hidden" name="id" value="<?= $product['_id'] ?>">

    <div>
        <a href="products" class="cancel">Anuluj</a>
        <input type="submit" value="Potwierdź"/>
    </div>
</form>

<?php include "fragments/layout/footer.php"; ?>

</body>
</html>
