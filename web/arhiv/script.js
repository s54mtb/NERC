// Function to load the header and footer directly into the page
function loadExternalHTML() {
    // Directly insert header HTML into the header container
    const headerContainer = document.getElementById('header-container');
    headerContainer.innerHTML = `
        <header>
            <div class="header-content">
                <h1>My Web Application</h1>
                <div class="theme-switcher">
                    <label for="theme-toggle">Toggle Dark/Light Theme</label>
                    <input type="checkbox" id="theme-toggle" />
                </div>
            </div>
        </header>
    `;

    // Directly insert footer HTML into the footer container
    const footerContainer = document.getElementById('footer-container');
    footerContainer.innerHTML = `
        <footer>
            <div class="footer-content">
                <p>&copy; 2025 My Web Application</p>
            </div>
        </footer>
    `;
}

// Toggle the theme between dark and light
document.getElementById('theme-toggle').addEventListener('change', function() {
    if (this.checked) {
        document.body.classList.remove('light-theme');
        document.body.classList.add('dark-theme');
        localStorage.setItem('theme', 'dark');
    } else {
        document.body.classList.remove('dark-theme');
        document.body.classList.add('light-theme');
        localStorage.setItem('theme', 'light');
    }
});

// Initialize page
document.addEventListener('DOMContentLoaded', function() {
    // Set initial theme based on localStorage
    const savedTheme = localStorage.getItem('theme');
    if (savedTheme === 'dark') {
        document.body.classList.add('dark-theme');
        document.getElementById('theme-toggle').checked = true;
    } else {
        document.body.classList.add('light-theme');
    }
});
	
		
			
				
					



