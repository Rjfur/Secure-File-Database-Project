<?php
extract($_REQUEST);
include("config.php");
session_start();
$sql = "DELETE FROM user_files WHERE file='$del'";
$result = mysqli_query($link,$sql) or die ( mysqli_error($link));

if($result) {

echo "<script>alert('file deleted successfully');
			window.location.href= 'allfiles.php';</script>";
}

?>