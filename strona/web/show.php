<?php

    $mongo = new MongoClient(
        "mongodb://localhost:27017/",
        [
            'username' => 'wai_web',
            'password' => 'w@i_w3b',
            'db' => 'wai',
        ]);

    $db = $mongo->wai;
	
	$user = [
        'login' => "asd",
        'password' => "dupa",
        'email' => "cos"
    ];
	
	//$db->userzy->insert($user);
	
	echo '<table>';
	foreach($db->userzy->find() as $user){
		echo '<tr>';
		echo "<td>".$user['login']."</td>";
		echo "<td>".$user['password']."</td>";
		echo "<td>aaaaaaaaa".$user['email']."</td>";
		echo '<tr>';
	}
	echo '</table>';
	echo 'aaa';
