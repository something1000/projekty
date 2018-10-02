<?php

    $mongo = new MongoClient(
        "mongodb://localhost:27017/",
        [
            'username' => 'wai_web',
            'password' => 'w@i_w3b',
            'db' => 'wai',
        ]);

    $db = $mongo->wai;
	

	
	//$db->userzy->insert($user);
	
	echo '<table>';
	foreach($db->images->find() as $img){
		echo '<tr>';
		echo "<td>".$img['author']."</td>";
		echo "<td>".$img['name']."</td>";
		echo "<td>".$img['private']."</td>";
		echo "<td>".$img['type']."</td>";
		echo '<tr>';
	}
	echo '</table>';
	
	// $db->images->remove();
