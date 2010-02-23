int main() {
	
	// Setup the loopback device.
/*	system("gnome-terminal \
			--default-working-directory=/home/eds/kernel \
			--command='sh setup.sh'");
*/	
	// Open the terminal.
	system("gnome-terminal \
			--default-working-directory=/home/eds/kernel \
			--geometry=+800+575 &");

	// Open the text editor.
	system("gedit &");
	
	// Open the file browser.
	system("nautilus \
			--geometry=800x550+475+0 &");

	// Return without error.
	return 0;
}

