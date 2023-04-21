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

//echo "Content-Length: 760343\r\n";


// Nom du fichier à créer
//$nomFichier = "/home/sebastien/42/monfichiertest.txt";

// Ouvre le fichier en mode écriture, crée le s'il n'existe pas
//$handle = fopen($nomFichier, 'w') or die("Impossible de créer le fichier!");

// Écrit du texte dans le fichier
//$texte = "Bonjour, ceci est un exemple de fichier texte créé avec PHP.";
//fwrite($handle, $texte);

// Ferme le fichier
//fclose($handle);

// Affiche un message de confirmation
//echo "Le fichier $nomFichier a été créé avec succès!";

echo "Début de l'attente...\n";
sleep(20);
?>

</body></html>