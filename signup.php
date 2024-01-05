<?php
include 'db.php';

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST['username'];
    $password = $_POST['password'];

    $hashed_password = password_hash($password, PASSWORD_DEFAULT);

    $sql = "INSERT INTO users (username, password) VALUES ('$username', '$hashed_password')";

    if ($conn->query($sql) === TRUE) {
        // Registration successful
        echo "Registered successfully!";
    } else {
        // Handle error
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
}
?>
