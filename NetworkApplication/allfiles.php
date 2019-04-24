<?php
session_start();
include("config.php");
include("header.php");
?>
<!DOCTYPE html>
<html>
<head>
	
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.css">
	<title>ALL FILES</title>
	<link rel="stylesheet" href="style.css" type="text/css" />
</head>
<div id="body">
<table> 
<tr>
<th> ALL files </th> <th> <button><a href="welcome.php" style="font-size: 16px;" > UPLOAD NEW FILE </a></button>
</th>
</tr>
<tr>
<td>
Filename 
</td>
<td>
Filetype
</td>
<td>
Filesize 
</td>
<td>
Download File
</td>
<td>
Delete File
</td>
<td>
	Share To
</td>
<td>
	Share File
</td>
</tr>

<?php
	

	$id = htmlspecialchars($_SESSION["id"]);
	$q = "SELECT * FROM users";
$users = mysqli_query($link,$q) or die (mysqli_error($link));
$query = "SELECT * FROM user_files where id='$id'";
$result =  mysqli_query($link, $query) or die (mysqli_error($link));
		//$row2 = mysqli_fetch_array($users);

if (mysqli_num_rows($result) > 0) {

	while ($row = mysqli_fetch_array($result)){ 
		// $rows = array($row2);
        $filename = $row['file'];
        $filetype = $row['type'];
        $filesize = $row['size'];
        echo "
        <tr>
        <td>$filename</td>
        <td>$filetype</td>
        <td>$filesize KB</td>
        <td><a href='uploads/$filename' target='_blank'> Download file </br></td>
        <td><a href='delete.php?del=$filename' class='link'>Delete</td>
        <td><select id='userSelectId'>";
        while($row2=mysqli_fetch_array($users)){
      $select.='<option value="'.$row2['id'].'">'.$row2['username'].'</option>';
  }
$select.='</select>';
echo $select;													
       echo "<td> <a href=sharefile.php?share=$filename&userToshare=1;
>Share File </td>
    </tr>";
}
    
	// while ($row = mysqli_fetch_array($result)){ 
	// 	$filename = $row['file'];
	// 	$filetype = $row['type'];
	// 	$filesize = $row['size'];
	// 	echo "
	// 	<tr>
	// 	<td>$filename</td>
	// 	<td>$filetype</td>
	// 	<td>$filesize KB</td>
	// 	<td><a href='uploads/$filename' target='_blank'> Download file </br></td>
	// 	<td><a href='delete.php?del=$filename' class='link'>Delete</td> 
	// 	<td> <a href='shareFile.php?share=$filename' class='shareFile'>Share File </td>
	// 	</tr>";		
	// }

}
else {
	echo "<p>NO FILES UPLOADED YET</p>";
}
?>

<!-- <table> 
<tr>
<th> Shared files </th>
</tr>
<tr>
<td>
Filename 
</td>
<td>
Filetype
</td>
<td>
Filesize 
</td>
<td>
Download File
</td>
<td>
Delete File
</td>
</tr> -->
<!-- <?php


$query2 = "SELECT * FROM user_files where shared_users_id='$id'";
$result =  mysqli_query($link, $query2) or die (mysqli_error($link));
		//$row2 = mysqli_fetch_array($users);

if (mysqli_num_rows($result) > 0) {

	while ($row = mysqli_fetch_array($result)){ 
		// $rows = array($row2);
        $filename = $row['file'];
        $filetype = $row['type'];
        $filesize = $row['size'];
        echo "
        <tr>
        <td>$filename</td>
        <td>$filetype</td>
        <td>$filesize KB</td>
        <td><a href='uploads/$filename' target='_blank'> Download file </br></td>
        <td><a href='delete.php?del=$filename' class='link'>Delete</td>";	

}
}
else
{
	echo "<p> No files shared</p>";
}



?> -->
</table>

</div>
	<p>
     <a href="logout.php" class="btn btn-danger" style="margin-top:70px;">Sign Out</a>
    </p>
<script
  src="https://code.jquery.com/jquery-3.4.0.min.js"
  integrity="sha256-BJeo0qm959uMBGb65z40ejJYGSgR7REI4+CW1fNKwOg="
  crossorigin="anonymous"></script>
</html>