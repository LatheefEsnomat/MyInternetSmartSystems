<?php
include 'db.php';

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST['username'];
    $password = $_POST['password'];

    $sql = "SELECT * FROM users WHERE username='$username'";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        if (password_verify($password, $row['password'])) {
            // Login successful
            echo "Login successful!";
        } else {
            // Invalid password
            echo "Invalid password!";
        }
    } else {
        // Username not found
        echo "Username not found!";
    }
}
?>
