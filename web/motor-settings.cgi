t <!DOCTYPE html>
t <html lang="en">
t <head>
t     <meta charset="UTF-8">
t     <meta name="viewport" content="width=device-width, initial-scale=1.0">
t     <title>Motor Driver Settings</title>
t     <link rel="stylesheet" href="style.css">
t </head>
t <body>
t 
t     <!-- Header Section -->
t     <header>
t         <a href="index.html">
t             <img src="banner_s54MTB.png" alt="S54MTB Banner" class="banner">
t         </a>
t     </header>
t 
t     <!-- Main Container for Form -->
t     <div class="container">
t         <h1>Motor Driver Settings</h1>
t 
t         <!-- Driving Method Switch -->
t         <label for="drivingMethodRelays">Drive with Relays:</label>
t         <input type="radio" id="drivingMethodRelays" name="drivingMethod" 
t            value="relays" onclick="toggleDrivingMethod()" checked>
t         
t         <label for="drivingMethodHBridge">Drive with H-Bridge:</label>
t         <input type="radio" id="drivingMethodHBridge" name="drivingMethod" 
t            value="hbridge" onclick="toggleDrivingMethod()">
t         
t         <br><br>
t 
t         <form action="#" method="post">
t             <!-- Group 1: Motor Control with Relays -->
t             <fieldset id="relaysSettings">
t                 <legend>Motor Control with Relays</legend>
t 
t                 <div class="form-group">
t                     <label for="relayType">Relay driving Type:</label>
t                     <select id="relayType" name="relayType" onchange="toggleRelaySettings()">
t                         <option value="1">REL1: Direction / REL2: Power</option>
t                         <option value="2">REL1: Direction 1 / REL2: Direction 2</option>
t                     </select>
t                 </div>
t 
t                 <div id="relaySettings1" style="display:none;">
t                     Direction/Power settings <br> <br>
t                         <label for="direction">Direction:</label>
t                         <select id="direction" name="direction">
t                             <option value="cw-ccw">CW/CCW</option>
t                             <option value="ccw-cw">CCW/CW</option>
t                         </select> <br>
t                  
t                         <label for="powerRelayInv">Power Relay inverted:</label>
t                         <input type="checkbox" id="powerRelay" name="powerRelayInv">
t                     
t                 </div>
t 
t                 <div id="relaySettings2" style="display:none;">
t                     Dir1 / Dir2 Settings <br><br>
t                         <label for="cwRelay">REL1:CW, REL2: CCW</label>
t                         <input type="radio" id="cwRelay" name="directionRelay" checked> <br>
t                         <label for="ccwRelay">REL1:CCW, REL2: CW</label>
t                         <input type="radio" id="ccwRelay" name="directionRelay">
t                 </div>
t             </fieldset>
t 
t             <!-- Group 2: Motor Control with H-Bridge -->
t             <fieldset id="hbridgeSettings" style="display:none;">
t                 <legend>Motor Control with H-Bridge</legend>
t 
t                 <div class="form-group">
t                     <label for="acceleration">Ramp-up Acceleration (% of full speed per second):</label>
t                     <input type="number" id="acceleration" name="acceleration" min="0" max="100" value="50">
t                 </div>
t 
t                 <div class="form-group">
t                     <label for="deceleration">Ramp-down Deceleration (% of full speed per second):</label>
t                     <input type="number" id="deceleration" name="deceleration" min="0" max="100" value="50">
t                 </div>
t 
t                 <div class="form-group">
t                     <label for="startSpeed">Initial Speed (% of full speed):</label>
t                     <input type="number" id="startSpeed" name="startSpeed" min="0" max="100" value="20">
t                 </div>
t 
t                 <div class="form-group">
t                     <label for="pwmFrequency">PWM Frequency (Hz):</label>
t                     <input type="number" id="pwmFrequency" name="pwmFrequency" min="5000" max="30000" value="15000">
t                 </div>
t 
t                     <label for="invertOutput">Invert H-Bridge Output:</label>
t                     <input type="checkbox" id="invertOutput" name="invertOutput">
t 
t                 <div class="form-group">
t                     <label for="nominalCurrent">Nominal Motor Current (A):</label>
t                     <input type="number" id="nominalCurrent" name="nominalCurrent" min="0" step="0.1" max="6.0" value="1.5">
t                 </div>
t 
t                 <div class="form-group">
t                     <label for="minCurrentAlert">Min Current Alert (% of nominal current):</label>
t                     <input type="number" id="minCurrentAlert" name="minCurrentAlert" min="0" max="100" value="20">
t                 </div>
t 
t                 <div class="form-group">
t                     <label for="maxCurrentAlert">Max Current Alert (% of nominal current):</label>
t                     <input type="number" id="maxCurrentAlert" name="maxCurrentAlert" min="100" max="200" value="150">
t                 </div>
t             </fieldset>
t 
t             <br><br>
t 
t             <button type="submit" class="button">Save Settings</button>
t         </form>
t     </div>
t 
t     <script>
t         // Function to toggle the visibility of relay and H-bridge settings based on the selected driving method
t         function toggleDrivingMethod() {
t             const relaysSettings = document.getElementById("relaysSettings");
t             const hbridgeSettings = document.getElementById("hbridgeSettings");
t 
t             if (document.getElementById("drivingMethodRelays").checked) {
t                 relaysSettings.style.display = "block";
t                 hbridgeSettings.style.display = "none";
t             } else {
t                 relaysSettings.style.display = "none";
t                 hbridgeSettings.style.display = "block";
t             }
t         }
t 
t         // Function to toggle the relay settings based on the selected relay type
t         function toggleRelaySettings() {
t             var relayType = document.getElementById("relayType").value;
t             if (relayType === "1") {
t                 document.getElementById("relaySettings1").style.display = "block";
t                 document.getElementById("relaySettings2").style.display = "none";
t             } else if (relayType === "2") {
t                 document.getElementById("relaySettings1").style.display = "none";
t                 document.getElementById("relaySettings2").style.display = "block";
t             }
t         }
t 
t         // Initialize the form with the default selected driving method (relays)
t         window.onload = function() {
t             toggleDrivingMethod();
t         };
t        window.onload = toggleRelaySettings;
t     </script>
t </body>
t </html>
.