<?php

    if (!isset($_COOKIE['alsaamenagements'])) {
        if ($_GET['rq']) {
            $qr = $_GET['rq'];
            /*
            1 = ll
            2 = st
            3 = fl
            4 = fb
            5 = ig
            6 = tw
            7 = lw
            8 = cv
            9 = pn
            */ }
        else {
            $qr = 10; }    
        setcookie('alsaamenagements',$qr , (time()+3600*24), '/', '', false,true);
        setVisite($qr);
    }

function dbConnect($sqlReq, $ecriture) {
    $host = '127.0.0.1';
    $port = '3306';
    $user = 'alsa';
    $pwd = 'Mesbuff..12';
    $dbname = 'alsa_web';
    
    $conn = new mysqli($host, $user, $pwd, $dbname);
    
    if ($conn->connect_error) {
        die("Erreur de connection: " . $conn->connect_error);
    }   
    // echo "Connecté..";

    if ($ecriture) {
        if ($conn->query($sqlReq) === TRUE) {
            // echo "Enregistrement effectué";
        }
        else {
            echo "Erreur: " . $sqlReq . "<br>" . $conn->error;
        }
    }
    else {
        $sqlResult = $conn->query($sqlReq);
        return $sqlResult;
        $sqlResult->free();
    }
    $conn->close();
}

function referenceRecup() {
    $sqlReq = "SELECT * FROM reference ORDER BY RAND() LIMIT 10";
    $sqlResult = dbConnect($sqlReq , false);
    while ($reference = $sqlResult->fetch_assoc()) {
            echo '<div>';
            echo '<div class="testimonial">';
            echo '  <figure class="mb-4">';
            echo '    <img src="images/' . $reference['dir_photo'] . '" alt="Alsa Aménagements Extérieur ' . $reference['nom_p'] . ' avis" class="img-fluid mb-3">';
            echo '    <p>' . $reference['nom_p'] . '</p>';
            echo '  </figure>';
            echo '  <blockquote>';
            echo '    <p>' . $reference['texte'] . '</p>';
            echo '  </blockquote>';
            echo ' </div>';    
            echo '</div>';
    }
    unset($sqlReq);
    unset($reference);
    unset($sqlResult);
}

function setVisite($qrCode) {
    $sqlReq = "INSERT INTO compteur (date, heure, type) VALUES (CURDATE(), CURTIME(), '$qrCode' )";
    dbConnect($sqlReq, true);
    unset($sqlReq);
}

function news() {    
    $sqlReq = "SELECT * FROM `news-short` ORDER BY date LIMIT 6";

    $sqlReslut = dbConnect($sqlReq, false);
    while ($fetchingSQL = $sqlReslut->fetch_assoc()) {
        echo '<div class="col-md-6 col-lg-6 mb-4 mb-lg-4">';
        echo '   <div class="h-entry">';
        echo '     <img src="images/' . $fetchingSQL['photo'] . '" alt="Alsa Aménagements ' . $fetchingSQL['titre'] . ' - Signature extérieur" class="img-fluid">';
        echo '     <h2 class="font-size-regular"><a href="#">' . $fetchingSQL['titre'] . '</a></h2>';
        echo '     <div class="meta mb-4">par ' . $fetchingSQL['auteur'] . ' <span class="mx-2">&bullet;</span> ' . $fetchingSQL['date'] . ' à 00:00 <span class="mx-2">&bullet;</span> <a href="#">Lire</a></div>';
        echo '     <p>' . $fetchingSQL['texte'] . '</p>';
        echo '   </div>';
        echo ' </div>';
    }
    unset($sqlReq);
    unset($fetchingSQL);
    unset($sqlResult);
}

// Fonctions concernant les demandes de rappel ou contact
function setDemande($nomProspect, $telephoneProsp) {
    $sqlReq = "INSERT INTO rappel (nom, telephone) VALUES ('$nomProspect', '$telephoneProsp')";
    dbConnect($sqlReq,true);
    unset($sqlReq);
}

function setContact($prenom, $nom, $mail, $subject, $message) {
    $sqlReq = "INSERT INTO contact (nom, prenom, email, sujet, message) VALUES ('$nom', '$prenom', '$mail', '$subject', '$message')";
    dbConnect($sqlReq,true);
    unset($sqlReq);
}

?>


