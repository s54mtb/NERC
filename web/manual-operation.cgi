t <!DOCTYPE html>
t <html lang="en">
t <head>
t    <meta charset="UTF-8">
t    <meta name="viewport" content="width=device-width, initial-scale=1.0">
t    <title>Manual operation</title>
t    <link rel="stylesheet" href="style.css">
t </head>
t <body>
t    <!-- Banner Image -->
t    <header>
t        <a href="index.html"><img src="banner_s54MTB.png" alt="S54MTB Banner" class="banner"></a>
t    </header>
t    <div class="container">
t      <div class="form-group">
t        <table> <tr>
t          <td><div class="knob-container">
t            <div class="knob" id="knob"></div>
t            </div></td>
t            <td><button type="button" class="button">GO!</button></td>
t          </tr></table>
t        </div>
t      <div class="form-group">
t        <input 
t            type="range" 
t            id="angleInput" 
t            class="range-input" 
t            min="0" 
t            max="360" 
t            value="0"
t        >
t        <div class="angle-display" id="angleDisplay">Angle: 0&deg;</div>
t      </div>
t    </div>
t    <script>
t        const knob = document.getElementById('knob');
t        const angleInput = document.getElementById('angleInput');
t        const angleDisplay = document.getElementById('angleDisplay');
t
t        // Update the knob rotation based on the range input value
t        angleInput.addEventListener('input', () => {
t            const angle = angleInput.value;
t            knob.style.transform = `rotate(${angle}deg)`;
t            angleDisplay.textContent = `Angle: ${angle}`+String.fromCharCode(176);
t        });
t    </script>
t </body>
t </html>
.