t <!DOCTYPE html>
t <html lang="en">
t <head>
t    <meta charset="UTF-8">
t    <meta name="viewport" content="width=device-width, initial-scale=1.0">
t    <title>System Settings</title>
t    <link rel="stylesheet" href="style.css">
t    <script language=JavaScript>
t      function changeConfirm(f){
t        if(!confirm('Ali bi res rad\nspremenil geslo?')) return;
t        f.submit();
t      }
t     // Function to toggle visibility of password
t     function togglePasswordVisibility() {
t       const passwordField = document.getElementById('password');
t       const passwordField2 = document.getElementById('password2');
t       const checkbox = document.getElementById('visible');
t       // Switch between 'text' in 'password'
t       if (checkbox.checked) {
t          passwordField.type = 'text';
t          passwordField2.type = 'text';
t       } else {
t          passwordField.type = 'password';
t          passwordField2.type = 'password';
t       }
t     }
t    </script>
t </head>
t <body>
t    <!-- Banner Image -->
t    <header>
t        <a href="index.html"><img src="banner_s54MTB.png" alt="S54MTB Banner" class="banner"></a>
t    </header>
t    <div class="container">
t        <h1>System Settings</h1>
t        <form action="index.html" method="post" name="loginForm">
t            <div class="form-group">
t                <label for="username">Username:</label>
c b u <input type="text" id=username name=username  size=16 maxlength=16 value="%s">
t            </div>
t            <div class="form-group">
t                <label for="password">Password:</label>
c b p <input type=password name=password id=password size=16 maxlength=16 value="%s">
t            </div>
t            <div class="form-group">
t                <label for="password2">Repeat password:</label>
c b q <input type=password name=password2 id=password2 size=16 maxlength=16 value="%s">
t            </div>
t            <div class="form-group">
t                <label for="visible">Show passwords:</label>
t                <input type="checkbox" id="visible" onclick="togglePasswordVisibility()"> 
t            </div>
t            <button type="submit" class="button" onclick="changeConfirm(this.form)">Change</button>
t        </form>
t    </div>
t    <!-- Footer -->
t    <footer>  <hr>
t        <p>Author: <strong>S54MTB - Marko Pavlin</strong></p>
t        <p>Find me on <a href="https://github.com/s54mtb" target="_blank">GitHub</a> | 
t            Check my <a href="https://www.qrz.com/db/S54MTB" target="_blank">QRZ.com</a></p>
t    </footer>
t </body>
t </html>
.