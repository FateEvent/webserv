<?php
    session_start();

    $id_session = session_id();
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Session PHP</title>
        <meta charset="utf-8">
    </head>
    
    <body>
        <h1>Session PHP-CGI</h1>
        <?php
            if($id_session)
			{
                echo 'ID found! id session: ' .$id_session. '<br>';
            }
            echo '<br><br>';
            if(isset($_COOKIE['PHPSESSID']))
			{
                echo 'ID cookie : <br>' .$_COOKIE['PHPSESSID'];
            }

			if(isset($_SESSION['test']))
			{
                echo 'The value of test in your session is: ' .$_SESSION['test']. '<br>';
			}
			else
			{
				echo 'Add a variable "test".<br>';
				$_SESSION['test'] = 'My test should work!';
			}
        ?>
    </body>
</html>