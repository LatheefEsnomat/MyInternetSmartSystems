<?php
$servername = "localhost";
// $username = "your_username";
$username = "root@localhost";
// $password = "your_password";
$password = "";
$dbname = "miss_data";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
?>
