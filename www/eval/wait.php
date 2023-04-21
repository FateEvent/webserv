<?php
echo "content-type: text/html; charset=UTF-8\r\n\r\n";
?>

<!DOCTYPE html>
<!-- Contrbuted by GS -->
<html><head></head><body>
<?php
while (list($var,$value) = each ($_ENV)) {
    echo "$var => $value <br />";
}

echo "Début de l'attente...\n";
sleep(30);
echo "Fin de l'attente...\n";
echo "Probleme si vous lisez ceci!";
?>

</body></html>