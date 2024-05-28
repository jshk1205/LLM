document.getElementById('loginForm').addEventListener('submit', function(event) {
    event.preventDefault();

    var username = document.getElementById('username').value;
    var password = document.getElementById('password').value;

    // Here you can perform client-side validation if needed

    // Sending login credentials to the server
    fetch('/login', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({username: username, password: password})
    })
    .then(response => response.json())
    .then(data => {
        if (data.success) {
            window.location.href = '/dashboard'; // Redirect to dashboard if login successful
        } else {
            document.getElementById('message').innerText = data.message; // Display error message
        }
    })
    .catch(error => {
        console.error('Error:', error);
    });
});
