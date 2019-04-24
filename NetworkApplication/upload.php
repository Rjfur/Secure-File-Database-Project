<?php
    include_once 'config.php';
    session_start();

if (isset($_POST['submit']))
{
	$id = htmlspecialchars($_SESSION["id"]);
	

	$file = $_FILES['file']['name'];
	$file_loc=  $_FILES['file']['tmp_name'];
	$file_size  = $_FILES['file']['size'];
	$file_type = $_FILES['file']['type'];
	$folder = "uploads/";
	$new_file_name  = strtolower($file);
	$final_file = str_replace(' ', '-', $new_file_name);
	if ($file_size > 50000000)  {
		echo "<script>alert('file size too big');
		window.location.href='view.php';</script>";
	}
	else if ($file_size == 0 ) {
	echo "<script>alert('select a file to upload');
	window.location.href= 'welcome.php';</script>";
}
	else if ($file_size <= 50000000) {
		move_uploaded_file($file_loc, $folder.$final_file);
		$new_size = $file_size / 1024;
		$query = "INSERT into user_files(id,file,type,size) VALUES ('$id','$final_file' , '$file_type', '$new_size')" ;
		$result = mysqli_query($link , $query) or die (mysqli_error($link));
		if ($result) {
			echo "<script>alert('file uploaded successfully');
			window.location.href= 'allfiles.php';</script>";
		}
	}
	else {
		echo "<script>alert('error');
		window.location.href= 'welcome.php';</script>";
	}
}

?>