


enum
  {
  		CMD_MAC,
		CMD_IP,
		CMD_DHCP,
		CMD_MASK,
		CMD_GATEWAY,
		CMD_DNS1,
		CMD_DNS2,
		CMD_HOSTNAME,
		CMD_N1MM_IP,
		CMD_N1MM_PORT,
		CMD_USERNAME,
		CMD_PASSWORD,
		CMD_PASSWORD2,
		CMD_ANGLE_0,
		CMD_ANGLE_1,
		CMD_ANGLE_2,
		CMD_ADC_0,
		CMD_ADC_1,
		CMD_ADC_2,
// Add more 
CMD_LAST
};
	

typedef enum 
{
	IP_OWN,
	IP_MASK,
	IP_GATEWAY,
	IP_DNS1,
	IP_DNS2,
	IP_N1MM
} target_ip_t;


typedef enum 
{
	PWD_USER, 
	PWD_PASS1,
	PWD_PASS2
} pwd_seetings_t;



struct cmd_st
{
const char *cmdstr;
int id;
};

void cmd_MAC(char *value);
void cmd_IP(char *value, target_ip_t target_ip);
void cmd_DHCP(char *value);
void cmd_MASK(char *value);
void cmd_GATEWAY(char *value);
void cmd_DNS1(char *value);
void cmd_DNS2(char *value);
void cmd_HOSTNAME(char *value);
void cmd_N1MM_IP(char *value);
void cmd_N1MM_PORT(char *value);
void cmd_PWD(char *value, pwd_seetings_t type);
void cmd_ANGLE_0(char *value);
void cmd_ANGLE_1(char *value);
void cmd_ANGLE_2(char *value);
void cmd_ADC_0(char *value);
void cmd_ADC_1(char *value);
void cmd_ADC_2(char *value);


