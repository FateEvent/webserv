<?php
   include('general.php');

   // Les données sont en retour J-SON
   header('Content-Type: application/json');

   // Données récupérés via POST
  $to = "st@alsa-amenagements.fr";
  $name = $_POST['name'];


  if ($_POST['rappel'] == 'false' ) {
   $prenom = $_POST['prenom'];
   $email = $_POST['email'];
   $sujet = $_POST['sujet'];
   $postmessage = $_POST['message']; 
   $subject = "Contact via site Web";

   // Email Template
   $message = "<b>Nom : </b>". $name ."<br>";
   $message .= "<b>Prénom : </b>". $prenom ."<br>";
   $message .= "<b>Adresse mail : </b>".$email."<br>";
   $message .= "<b>Sujet : </b>".$sujet."<br>";
   $message .= "<b>Message : </b>".$postmessage."<br>";

   // INSCRIPTION BDD
   setContact($prenom, $name, $email, $sujet, $postmessage);
  }
  else {
   $contact = $_POST['contact'];
   $subject = "Demande de rappel";
   // Email Template
   $message = "<b>Nom : </b>". $name ."<br>";
   $message .= "<b>Téléphone du contact : </b>".$contact."<br>";

   // INSCRIPTION BDD
   setDemande($name, $contact);
  }

   // Entete mail
   $header = "From:"+$email+" \r\n";
   $header .= "MIME-Version: 1.0\r\n";
   $header .= "Content-type: text/html\r\n";
   $retval = mail ($to,$subject,$message,$header);  

   // Retour message de notification
   if( $retval == true ) {
      echo json_encode(array(
         'success'=> true,
         'message' => 'Votre demande est prise en compte.'
      ));
   }else {
      echo json_encode(array(
         'error'=> true,
         'message' => 'Une erreur est survenue, veuillez réesayer plus tard. Merci'
      ));
   }
?>