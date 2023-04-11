 $(document).ready(function(){
    // au clic sur un lien
    $('a.scroll').on('click', function(evt){
 // bloquer le comportement par défaut: on ne rechargera pas la page
       evt.preventDefault(); 
       // enregistre la valeur de l'attribut  href dans la variable target
 var target = $(this).attr('href');
       /* le sélecteur $(html, body) permet de corriger un bug sur chrome 
       et safari (webkit) */
 $('html, body')
       // on arrête toutes les animations en cours 
       .stop()
       /* on fait maintenant l'animation vers le haut (scrollTop) vers 
        notre ancre target */
       .animate({scrollTop: $(target).offset().top}, 1300 );
    });
});

// #sendMailForm takes the data from the form with given ID
$( '#sendMailForm' ).submit(function ( e ) {
    var data = {
        'prenom' : $('#prenom').val(),
        'name': $('#name').val(),
        'email': $('#email').val(),
        'sujet': $('#sujet').val(),
        'message' : $('#message').val(),
        'rappel' : 'false',
    };
    
    var $waitContactDiv = $(".waitContact");
    var $btnEnvoiContact = $(".envoiebtn");
    $waitContactDiv.show();
    $btnEnvoiContact.hide();
    
    // POST data to the php file
    $.ajax({ 
        url: 'functions/mail.php', 
        data: data,
        type: 'POST',
        success: function (data) {
			// For Notification
            document.getElementById("sendMailForm").reset();
            var $alertDiv = $(".mailResponse");
            $waitContactDiv.hide();
            $btnEnvoiContact.show();
            $alertDiv.show();
            $alertDiv.find('.alert').removeClass('alert-danger alert-success');
            $alertDiv.find('.mailResponseText').text("");
            if(data.error){
                $alertDiv.find('.alert').addClass('alert-danger');
                $alertDiv.find('.mailResponseText').text(data.message);
            }else{
                $alertDiv.find('.alert').addClass('alert-success');
                $alertDiv.find('.mailResponseText').text(data.message);
            }
        }
    });
    e.preventDefault();
});

// #sendRappel Récupère les données via le formulaire
$( '#sendRappel' ).submit(function ( e ) {
    var data = {
        'name': $('#nomProspect').val(),
        'contact': $('#telephone').val(),
        'rappel': 'true',
    };
    
    var $waitTelDiv = $(".waitSend");
    var $btnEnvoiTel = $('.telbtn');
    $waitTelDiv.show();
    $btnEnvoiTel.hide();

    // POST data to the php file
    $.ajax({ 
        url: 'functions/mail.php', 
        data: data,
        type: 'POST',
        success: function (data) {
			// For Notification
            document.getElementById("sendRappel").reset();
            var $alertDiv = $(".telResponse");
            $waitTelDiv.hide();
            $btnEnvoiTel.show();
            $alertDiv.show();
            $alertDiv.find('.alert').removeClass('alert-danger alert-success');
            $alertDiv.find('.telResponseText').text('<span class="icon-spinner icon-spin-pulse"></span> Envoi en cours."');
            if(data.error){
                $alertDiv.find('.alert').addClass('alert-danger');
                $alertDiv.find('.telResponseText').text(data.message);
            }else{
                $alertDiv.find('.alert').addClass('alert-success');
                $alertDiv.find('.telResponseText').text(data.message);
            }
        }
    });
    e.preventDefault();
});