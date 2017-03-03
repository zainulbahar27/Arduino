void setup()
{
  // put your setup code here, to run once:
  system("telnetd -l /bin/sh"); //Start the telnet server on Galileo
  system("ifconfig eth0 169.254.1.1 netmask 255.255.0.0 up");
 
}
void loop()
{

}
