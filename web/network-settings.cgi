t <!DOCTYPE html>
t <html lang="en">
t <head>
t    <meta charset="UTF-8">
t    <meta name="viewport" content="width=device-width, initial-scale=1.0">
t    <title>Network Settings</title>
t    <link rel="stylesheet" href="style.css">
t    <script>
t        // JavaScript to enable/disable IP settings based on DHCP selection using direct references
t        function toggleIPFields() {
t            const ipField = document.getElementById('ip');
t            const gwField = document.getElementById('gateway');
t            const maField = document.getElementById('mask');
t            const dns1Field = document.getElementById('dns1');
t            const dns2Field = document.getElementById('dns2');
t            const checkbox = document.getElementById('dhcp');
t            // Switch fields
t            ipField.disabled = checkbox.checked;
t            maField.disabled = checkbox.checked;
t            gwField.disabled = checkbox.checked;
t            dns1Field.disabled = checkbox.checked;
t            dns2Field.disabled = checkbox.checked;
t        }
t        window.onload = toggleIPFields;
t    </script>
t </head>
t <body>
t    <!-- Banner Image -->
t    <header>
t        <a href="index.html"><img src="banner_s54MTB.png" alt="S54MTB Banner" class="banner"></a>
t    </header>
t    <div class="container">
t        <h1>Network Settings</h1>
t        <form action=network-settings.cgi method=post name=cgi>
t            <div class="form-group">
t                <label for="mac">MAC Address:</label>
c a v <input type="text" id="mac" name="mac" value="%s" size=18 maxlength=18 disabled=yes>
t            </div>
t            <div class="form-group">
t                <label for="dhcp">Enable DHCP:</label>
c a j <input type="checkbox" id="dhcp" name="dhcp" value="%s" onclick="toggleIPFields()">
t            </div>
t            <div class="form-group">
t                <label for="ip">IP Address:</label>
c a i <input type=text id=ip name=ip class=ip-field value="%s" size=18 maxlength=18>
t            </div>
t            <div class=form-group>
t                <label for=mask>Network Mask:</label>
c a m <input type=text id=mask name=mask class=ip-field value="%s" size=18 maxlength=18>
t            </div>
t            <div class=form-group>
t                <label for=gateway>Default Gateway:</label>
c a g <input type=text id=gateway name=gateway class=ip-field value="%s" size=18 maxlength=18>
t            </div>
t            <div class=form-group>
t                <label for=dns1>Primary DNS:</label>
c a p <input type=text id=dns1 name=dns1 class=ip-field value="%s" size=18 maxlength=18>
t            </div>
t            <div class=form-group>
t                <label for=dns2>Secondary DNS:</label>
c a s <input type=text id=dns2 name=dns2 class=ip-field value="%s" size=18 maxlength=18>
t            </div>
t            <div class=form-group>
t                <label for=hostname>Host Name:</label>
c a h <input type=text id=hostname name=hostname value="%s" size=16 maxlength=16>
t            </div>
t            <div class=form-group>
t                <label for=n1mm_ip>N1MM IP Address:</label>
c a n <input type=text id=n1mm_ip name=n1mm_ip class=ip-field value="%s" size=18 maxlength=18>
t            </div>
t            <div class=form-group>
t                <label for=n1mm_port>N1MM Port:</label>
c a r <input type=text id=n1mm_port name=n1mm_port class=ip-field value="%d" size=6 maxlength=6>
t            </div>
t            <button type="submit" class="button">Save Settings</button>
t        </form>
t    </div>
t    <!-- Footer -->
t    <footer> <hr>
t        <p>Author: <strong>S54MTB - Marko Pavlin</strong></p>
t        <p>Find me on <a href="https://github.com/s54mtb" target="_blank">GitHub</a> 
t           | Check my <a href="https://www.qrz.com/db/S54MTB" target="_blank">QRZ.com</a></p>
t    </footer>
t </body>
t </html>
.