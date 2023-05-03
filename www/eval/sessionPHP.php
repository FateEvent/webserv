<?php
    //On démarre une nouvelle session
    session_start();
    /*On utilise session_id() pour récupérer l'id de session s'il existe.
     *Si l'id de session n'existe  pas, session_id() rnevoie une chaine
     *de caractères vide*/
    $id_session = session_id();
    if(!isset($_COOKIE['PHPSESSID']))
    {
        echo "set-cookie: PHPSESSID=" . $id_session .";\r\n\r\n";
    }
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Session PHP</title>
        <meta charset="utf-8">
    </head>
    
    <body>
        <h1>Titre principal</h1>
        <?php
            if($id_session)
			{
                echo 'ID de session récupéré via session_id : <br>' .$id_session. '<br>';
            }
            echo '<br><br>';
            if(isset($_COOKIE['PHPSESSID']))
			{
                echo 'ID de session (récupéré via $_COOKIE) : <br>' .$_COOKIE['PHPSESSID'];
            }

			if(isset($_SESSION['test']))
			{
                echo 'Value of test: ' .$_SESSION['test']. '<br>';
			}
			else
			{
				echo 'Add in SESSION[test] -> My test should work!<br>';
				$_SESSION['test'] = 'My test should work!';
			}
        ?>
    </body>
</html>