t <!DOCTYPE html>
t <html lang="en">
t <head>
t    <meta charset="UTF-8">
t    <meta name="viewport" content="width=device-width, initial-scale=1.0">
t    <title>System status</title>
t    <link rel="stylesheet" href="style.css">
t </head>
t <body>
t    <!-- Banner Image -->
t    <header>
t        <a href="index.html"><img src="banner_s54MTB.png" alt="S54MTB Banner" class="banner"></a>
t    </header>
t    <div class="container">
t <h1>Configuration Display</h1>
t <table>
t 	<thead>
t 		<tr>
t 			<th>Parameter</th>
t 			<th>Value</th>
t 		</tr>
t 	</thead>
t 	<tbody>
t 		<tr>
t 			<td>PID_MaxSteps</td>
t 			<td>%d</td>
t 		</tr>
t 		<tr>
t 			<td>PID_pid</td>
t 			<td>{Kp: %4.1f, Ki: %4.1f, Kd: %4.1f}</td>
t 		</tr>
t 		<tr>
t 			<td>MOT_SpeedMin</td>
t 			<td>%4.1f</td>
t 		</tr>
t 		<tr>
t 			<td>MOT_SpeedMax</td>
t 			<td>%4.1f</td>
t 		</tr>
t 		<tr>
t 			<td>MOT_FinalPositionError</td>
t 			<td>%5.4f</td>
t 		</tr>
t 		<tr>
t 			<td>Rot_NumCalibPoints</td>
t 			<td>%d</td>
t 		</tr>
t 		<tr>
t 			<td>calib_points</td>
t           <td>
t              %s
t 			</td>
t 		</tr>
t 		<tr>
t 			<td>mac_addr</td>
t 			<td>%s</td>
t 		</tr>
t 		<tr>
t 			<td>ip_addr</td>
t 			<td>%s</td>
t 		</tr>
t 		<tr>
t 			<td>def_gw</td>
t 			<td>%s</td>
t 		</tr>
t 		<tr>
t 			<td>net_mask</td>
t 			<td>%s</td>
t 		</tr>
t 		<tr>
t 			<td>pri_dns</td>
t 			<td>%s</td>
t 		</tr>
t 		<tr>
t 			<td>sec_dns</td>
t 			<td>%s</td>
t 		</tr>
t 		<tr>
t 			<td>host_name</td>
t 			<td>%s</td>
t 		</tr>
t 		<tr>
t 			<td>N1MM_IP</td>
t 			<td>%s</td>
t 		</tr>
t 		<tr>
t 			<td>N1MM_Port</td>
t 			<td>%d</td>
t 		</tr>
t 		<tr>
t 			<td>DHCP_enabled</td>
t 			<td>%s</td>
t 		</tr>
t 		<tr>
t 			<td>password</td>
t 			<td>******</td>
t 		</tr>
t 		<tr>
t 			<td>username</td>
t 			<td>%s</td>
t 		</tr>
t 	</tbody>
t </table>
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