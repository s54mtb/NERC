t <html><head>
t <meta name="viewport" content="width=device-width, initial-scale=1.0">
t <title>System Settings</title>
t <script language=JavaScript>
t function changeConfirm(f){
t  if(!confirm('Ali bi res rad\nspremenil geslo?')) return;
t  f.submit();
t }
t // Funkcija za preklop tipa vnosnega polja
t function togglePasswordVisibility() {
t  const passwordField = document.getElementById('pw0');
t  const passwordField2 = document.getElementById('pw2');
t  const checkbox = document.getElementById('prikaziGeslo');
t  // Preklopi med 'text' in 'password'
t  if (checkbox.checked) {
t    passwordField.type = 'text';
t    passwordField2.type = 'text';
t  } else {
t    passwordField.type = 'password';
t    passwordField2.type = 'password';
t  }
t }
t </script></head>
i pg_header.inc
t <h2 align=center><br>Sistemske nastavitve</h2>
t <p><font size="2">Tu lahko nastavljaš 
t  <b>geslo</b>, za uporabniško ime <b>admin</b>. <br><br>
t  </font></p>
t <form action=index.html method=post name=cgi>
t <input type=hidden value="sys" name=pg>
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#aaccff>
t  <th width=40%>Parameter</th>
t  <th width=60%>Nastavitev</th></tr>
# Here begin data setting which is formatted in HTTP_Server_CGI.C module
c d 1 <tr><td>Avtentikacija</TD><TD><b>%s</b></td></tr>
t <tr><td>Geslo za uporabnika 'admin'</td>
c d 2 <td><input type=password name=pw0 id=pw0 size=10 maxlength=10 value="%s"></td></tr>
t <tr><td>Ponovi geslo</td>
c d 2 <td><input type=password name=pw2 id=pw2 size=10 maxlength=10 value="%s"></td></tr>
t <tr><td>Prikazuj gesla</td><td><input type="checkbox" 
t id="prikaziGeslo" onclick="togglePasswordVisibility()"></td></tr>
t </font></table>
# Here begin button definitions
t <p align=center>
t <input type=button name=set value="Potrditev" onclick="changeConfirm(this.form)">
t <input type=reset value="Preklic">
t </p></form>
i pg_footer.inc
.
