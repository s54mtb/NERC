t <!DOCTYPE html>
t <html lang="en">
t <head>
t    <meta charset="UTF-8">
t    <meta name="viewport" content="width=device-width, initial-scale=1.0">
t    <title>Calibration</title>
t    <link rel="stylesheet" href="style.css">
t    <script>
t        // JavaScript for handling slider and dynamic calibration point creation
t        function updateCalibrationPoints() {
t            const numPoints = document.getElementById('num_points').value;
t            const pointsContainer = document.getElementById('calibration_points');
t            pointsContainer.innerHTML = ''; // Clear existing points
t            for (let i = 0; i < numPoints; i++) {
t                const pointRow = document.createElement('div');
t                pointRow.classList.add('form-group');
t                pointRow.innerHTML = ' <table><tr>
t                    <td><label for="angle_'+i+'">Angle '+i+':</label>
t                    <input type="number" id="angle_'+i+'" name="angle_'+i+'" min="0" required></td>
t                    <td><label for="adc_'+i+'">ADC:</label>
t                    <input type="number" id="adc_'+i+'" name="adc_'+i+'" disabled></td>
t                    <td><button type="button" onclick="readADC('+i+')" class="button">Read ADC</button></td>
t                </tr></table><hr>';
t                pointsContainer.appendChild(pointRow);
t            }
t        }
t        function readADC(index) {
t            // Simulate ADC reading
t            const adcField = document.getElementById('adc_'+index);
t            adcField.value = Math.floor(Math.random() * 1024); // Random ADC value between 0 and 1023
t        }
t        window.onload = updateCalibrationPoints;
t    </script>
t </head>
t <body>
t    <!-- Banner Image -->
t    <header>
t        <a href="index.html"><img src="banner_s54MTB.png" alt="S54MTB Banner" class="banner"></a>
t    </header>
t    <div class="container">
t        <h1>Calibration</h1>
t        <!-- Step Input and Move Buttons -->
t        <div class="form-group">
t           <table><tr>
t            <td><button type="button" class="button" onclick="moveCCW()">Move CCW</button></td>
t            <td><label for="step">Step:</label>
t            <input type="number" id="step" name="step" required></td>
t            <td><button type="button" class="button" onclick="moveCW()">Move CW</button></td>
t          </tr></table>
t        </div>
t        <!-- Angle Min and Max Inputs -->
t        <div class="form-group">
t            <label for="angle_min">Angle Min:</label>
t            <input type="number" id="angle_min" name="angle_min" min="0" required>
t        </div>
t        <div class="form-group">
t            <label for="angle_max">Angle Max:</label>
t            <input type="number" id="angle_max" name="angle_max" min="0" required>
t        </div>
t        <!-- Calibration Points Slider -->
t        <div class="form-group">
t            <label for="num_points">Number of Calibration Points (2-10):</label>
t            <input type="range" id="num_points" name="num_points" min="2" max="10" value="2" 
t               oninput="updateCalibrationPoints()">
t            <span id="num_points_value">2</span>
t        </div>
t        <!-- Calibration Points -->
t        <div id="calibration_points">
t            <!-- Dynamic calibration points will be added here -->
t        </div>
t
t        <!-- Save and Cancel Buttons -->
t        <div class="form-group">
t            <button type="button" class="button">Save Calibration</button>
t            <button type="button" class="button">Cancel without Saving</button>
t        </div>
t    </div>
t    <!-- Footer -->
t    <footer>  <hr>
t        <p>Author: <strong>S54MTB - Marko Pavlin</strong></p>
t        <p>Find me on <a href="https://github.com/s54mtb" target="_blank">GitHub</a> | 
t           Check my <a href="https://www.qrz.com/db/S54MTB" target="_blank">QRZ.com</a></p>
t    </footer>
t </body>
t </html>
.