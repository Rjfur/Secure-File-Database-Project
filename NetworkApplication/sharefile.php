<?php
session_start();
include("config.php");
$id = htmlspecialchars($_SESSION["id"]);
echo 'Hello ' . htmlspecialchars($_GET["userToshare"]) . '!';
echo htmlspecialchars($_GET["share"]);	
echo $id;

?>