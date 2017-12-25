#define MAX_NFC_DATA_SIZE (600)
#define MAX_FW_BLOCK_SIZE (512)
#define MAX_NFC_RETRY (100)
#define MAX_READ_TIME (5)
#define NFC_READ_DELAY (20)
#define MAX_QUP_DATA_LEN (224)
#define MAX_CMD_LEN 100



typedef struct RF_entry_def {
	unsigned int RF_ID;
	unsigned int RF_Protocol;
	unsigned int RF_Technology;
} RF_Entry;

static struct device_info_def {
	unsigned int padding_exist;
	unsigned char padding;
	unsigned long fwVersion;
	unsigned int HW_model;
	unsigned int NCI_version;
	unsigned long NFCC_Features;
	unsigned char manufactor;

	unsigned char FW_Major;
	unsigned char FW_Minor;

	unsigned int protocol_set;
	unsigned int intf_set;
	unsigned int target_rf_id;
	unsigned int activated_INTF;
	unsigned int NTF_count;
	RF_Entry NTF_queue[15];
} gDevice_info;

typedef struct eeprom_package_def {
	uint8_t cmd[12];
} eeprom_package;

typedef struct control_msg_pack_def {
	uint8_t cmd[MAX_CMD_LEN];
	uint8_t exp_resp_content[MAX_CMD_LEN];
	uint8_t exp_ntf[MAX_CMD_LEN];
} control_msg_pack;

static control_msg_pack nfc_version_script[] = {
	{
		.cmd = { 3, 0x2F, 0x20, 0x00 },
		.exp_resp_content = { 1, 0x00 },
		.exp_ntf = { 0 },
	},
	{	/* PATCH_VERSION_CMD */
		.cmd = { 3, 0x2F, 0x1B, 0x00 },
		.exp_resp_content = { 1, 0x00 },
		.exp_ntf = { 0 },
	},
};

	static control_msg_pack nfc_version_script_test[] = {
		{	/* PATCH_VERSION_CMD */
			.cmd = { 3, 0x2F, 0x1B, 0x00 },
			.exp_resp_content = { 1, 0x00 },
			.exp_ntf = { 0 },
		},
		{
			/* CORE_RESET_CMD */
			.cmd = { 4, 0x20, 0x00, 0x01, 0x01 },
			.exp_resp_content = { 6, 0x40, 0x00, 0x03, 0x00, 0x10, 0x01 },
			.exp_ntf = { 0 },
		},
		{
			/* CORE_INIT_CMD */
			{ 3, 0x20, 0x01, 0x00 },
			{ 24, 0x40, 0x01, 0x15, 0x00, 0x01, 0x0E, 0x02, 0x00, 0x04, 0x00, 0x01, 0x02, 0x03, 0x03, 0xF8, 0x02, 0xFF, 0xF0, 0x01, 0x1A, 0x00, 0x00, 0x00, 0x00 },
			{ 0 },
		},
};

static control_msg_pack nfc_reader_A_script[] ={
	{
		/* CORE_RESET_CMD */
		.cmd = { 4, 0x20, 0x00, 0x01, 0x01 },
		.exp_resp_content = { 6, 0x40, 0x00, 0x03, 0x00, 0x10, 0x01 },
		.exp_ntf = { 0 },
	},
	{
		/* CORE_INIT_CMD */
		{ 3, 0x20, 0x01, 0x00 },
		{ 24, 0x40, 0x01, 0x15, 0x00, 0x01, 0x0E, 0x02, 0x00, 0x04, 0x00, 0x01, 0x02, 0x03, 0x03, 0xF8, 0x02, 0xFF, 0xF0, 0x01, 0x1A, 0x00, 0x00, 0x00, 0x00 },
		{ 0 }
	},
	{
		{ 7, 0x21, 0x00, 0x04, 0x01, 0x04, 0x01, 0x02 },
//		{ 4, 0x41, 0x00, 0x01, 0x00 },
		{ 1, 0x00 },
		{ 0 }
	},
	{
		{ 6, 0x21, 0x03, 0x03, 0x01, 0x00, 0x01 },
//		{ 4, 0x41, 0x03, 0x01, 0x00 },
		{ 1, 0x00 },
		{ 1, 0x01 }
	},
};

static control_msg_pack nfc_reader_B_script[] ={
	{
		.cmd = { 3, 0x2F, 0x20, 0x00 },
		.exp_resp_content = { 1, 0x00 },
		.exp_ntf = { 0 },
	},
	{
		{ 4, 0x20, 0x00, 0x01, 0x01 },
		{ 1, 0x00 },
		{ 0 },
	},
	{
		{ 3, 0x20, 0x01, 0x00 },
		{ 0 },
		{ 0 },
	},
	{
		{ 7, 0x21, 0x00, 0x04, 0x01, 0x04, 0x01, 0x02 },
		{ 1, 0x00 },
		{ 0 },
	},
	{
		{ 6, 0x21, 0x03, 0x03, 0x01, 0x01, 0x01 },
		{ 2, 0x00 },
		{ 1, 0x01 }
	},
};

static control_msg_pack nfc_reader_F_script[] ={
	{
		.cmd = { 3, 0x2F, 0x20, 0x00 },
		.exp_resp_content = { 1, 0x00 },
		.exp_ntf = { 0 },
	},
	{
		{ 4, 0x20, 0x00, 0x01, 0x01 },
		{ 1, 0x00 },
		{ 0 },
	},
	{
		{ 3, 0x20, 0x01, 0x00 },
		{ 0 },
		{ 0 },
	},
	{
		{ 7, 0x21, 0x00, 0x04, 0x01, 0x04, 0x01, 0x02 },
		{ 1, 0x00 },
		{ 0 },
	},
	{
		{ 6, 0x21, 0x03, 0x03, 0x01, 0x02, 0x01 },
		{ 2, 0x00 },
		{ 1, 0x01 }
	},
};

static control_msg_pack nfc_card_without_sim[] ={
//	{	/* PATCH_VERSION_CMD */
//		.cmd = { 3, 0x2F, 0x1B, 0x00 },
//		.exp_resp_content = { 9, 0x4F, 0x1B, 0x06, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00 },
//		.exp_ntf = { 0 },
//	},
	{
		.cmd = { 3, 0x2F, 0x20, 0x00 },
		.exp_resp_content = { 1, 0x00 },
		.exp_ntf = { 0 },
	},
	{
		/* CORE_RESET_CMD */
		.cmd = { 4, 0x20, 0x00, 0x01, 0x01 },
		.exp_resp_content = { 6, 0x40, 0x00, 0x03, 0x00, 0x10, 0x01 },
		.exp_ntf = { 0 },
	},
	{
		/* CORE_INIT_CMD */
		{ 3, 0x20, 0x01, 0x00 },
		{ 24, 0x40, 0x01, 0x15, 0x00, 0x01, 0x0E, 0x02, 0x00, 0x04, 0x00, 0x01, 0x02, 0x03, 0x03, 0xF8, 0x02, 0xFF, 0xF0, 0x01, 0x1A, 0x00, 0x00, 0x00, 0x00 },
		{ 0 }
	},
	{	/* CORE_SET_CONFIG_CMD */
		{ 46, 0x20, 0x02, 0x2B, 0x0B, 0x00, 0x02, 0xE8, 0x03, 0x32, 0x01, 0x20, 0x50, 0x01, 0x00, 0x51, 0x08, 0x20, 0x79, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x53, 0x02, 0x00, 0x01, 0x55, 0x01, 0x01, 0x58, 0x01, 0x07, 0xA0, 0x01, 0x01, 0xD0, 0x01, 0x64, 0xD1, 0x01, 0x28, 0xD2, 0x01, 0xFF },
//		{ 5 , 0x40, 0x02, 0x02, 0x00, 0x00 },
		{ 1, 0x00 },
		{ 0 },
	},
	{
		/* RF_DISCOVER_MAP */
		{ 10, 0x21, 0x00, 0x07, 0x02, 0x04, 0x03, 0x02, 0x05, 0x03, 0x03 },
//		{ 4, 0x41, 0x00, 0x01, 0x00 },
		{ 1, 0x00 },
		{ 0 }
	},
	{
		/* RF_SET_LISTEN_MODE_ROUTING*/
		{ 35, 0x21, 0x01, 0x20, 0x00, 0x06, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x03, 0x00, 0x01, 0x01, 0x00, 0x03, 0x00, 0x01, 0x02, 0x01, 0x03, 0x00, 0x01, 0x04, 0x01, 0x03, 0x00, 0x01, 0x05, 0x01, 0x03, 0x00, 0x01, 0x03 },
//		{ 4, 0x41, 0x03, 0x01, 0x00},
		{ 1, 0x00 },
		{ 0 },
	},
	{	/* RF_DISCOVER */
		{ 14, 0x21, 0x03, 0x0B, 0x05, 0x80, 0x01, 0x81, 0x01, 0x82, 0x01, 0x83, 0x01, 0x85, 0x01 },
//		{ 4, 0x41, 0x03, 0x01, 0x00 },
		{ 1, 0x00 },
		{ 1, 0x01 }
	},
};

static control_msg_pack felica_ese_without_sim[] ={
	{
		.cmd = { 3, 0x2F, 0x20, 0x00 },
		.exp_resp_content = { 1, 0x00 },
		.exp_ntf = { 0 },
	},
	{	/* PATCH_VERSION_CMD */
		.cmd = { 3, 0x2F, 0x1B, 0x00 },
		.exp_resp_content = { 1, 0x00 },
		.exp_ntf = { 0 },
	},
	{
		/* CORE_RESET_CMD */
		.cmd = { 4, 0x20, 0x00, 0x01, 0x01 },
		.exp_resp_content = { 6, 0x40, 0x00, 0x03, 0x00, 0x10, 0x01 },
		.exp_ntf = { 0 },
	},
	{
		/* CORE_INIT_CMD */
		{ 3, 0x20, 0x01, 0x00 },
		{ 24, 0x40, 0x01, 0x15, 0x00, 0x01, 0x0E, 0x02, 0x00, 0x04, 0x00, 0x01, 0x02, 0x03, 0x03, 0xF8, 0x02, 0xFF, 0xF0, 0x01, 0x1A, 0x00, 0x00, 0x00, 0x00 },
		{ 0 }
	},
	{	/* CORE_SET_CONFIG_CMD */
		{ 49, 0x20, 0x02, 0x2E, 0x0C, 0x00, 0x02, 0x2C, 0x01, 0x32, 0x01, 0x20, 0x50, 0x01, 0x00, 0x51, 0x08, 0x20, 0x79, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x53, 0x02, 0x00, 0x01, 0x55, 0x01, 0x01, 0x58, 0x01, 0x07, 0xA0, 0x01, 0x01, 0xD0, 0x01, 0x1E, 0xD1, 0x01, 0x19, 0xD2, 0x01, 0xFF, 0xD3, 0x01, 0x00 },
		{ 1, 0x00 },
		{ 0 },
	},
	{
		/* RF_DISCOVER_MAP */
		{ 10, 0x21, 0x00, 0x07, 0x02, 0x04, 0x03, 0x02, 0x05, 0x03, 0x03 },
		{ 1, 0x00 },
		{ 0 }
	},
//+
	{
		/* NFCEE_DISCOVER */
		{ 4, 0x22, 0x00, 0x01, 0x01 },
		{ 1, 0x00 },
		{ 1, 0x01 }
	},
	{
		/* CORE_CONN_CREATE */
		{ 9, 0x20, 0x04, 0x06, 0x03, 0x01, 0x01, 0x02, 0x01, 0x01 },
		{ 1, 0x00 },
		{ 0 }
	},
	{
		/* NFCEE_MODE_SET_CMD (SAM) */
		{ 5, 0x22, 0x01, 0x02, 0x02, 0x01 },
		{ 1, 0x00 },
		{ 1, 0x01 }
	},
	{
		/* RF_SET_LISTEN_MODE_ROUTING */
//		{ 40, 0x21, 0x01, 0x25, 0x00, 0x07, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x03, 0x00, 0x01, 0x01, 0x00, 0x03, 0x02, 0x03, 0x02, 0x01, 0x03, 0x00, 0x01, 0x04, 0x01, 0x03, 0x00, 0x01, 0x05, 0x01, 0x03, 0x02, 0x03, 0x03, 0x01, 0x03, 0x00, 0x01, 0x03 },
		{ 15, 0x21, 0x01, 0x0C, 0x00, 0x02, 0x00, 0x03, 0x02, 0x03, 0x02, 0x01, 0x03, 0x02, 0x03, 0x03 },
		{ 1, 0x00 },
		{ 0 },
	},
	{
		/* NFCEE_DISCOVER */
		{ 4, 0x22, 0x00, 0x01, 0x00 },
		{ 1, 0x00 },
		{ 0 },
	},
//-
	{	/* RF_DISCOVER */
//		{ 14, 0x21, 0x03, 0x0B, 0x05, 0x80, 0x01, 0x81, 0x01, 0x82, 0x01, 0x83, 0x01, 0x85, 0x01 },
		{ 6, 0x21, 0x03, 0x03, 0x01, 0x82, 0x01 },
		{ 1, 0x00 },
		{ 1, 0x01 }
	},
};

static control_msg_pack nfc_card_script[] ={
	{
		.cmd = { 4, 0x20, 0x00, 0x01, 0x01 },
		.exp_resp_content = { 1, 0x00 },
		.exp_ntf = { 0 },
	},
	{
		/* CORE_INIT_CMD */
		{ 3, 0x20, 0x01, 0x00 },
		{ 24, 0x40, 0x01, 0x15, 0x00, 0x01, 0x0E, 0x02, 0x00, 0x04, 0x00, 0x01, 0x02, 0x03, 0x03, 0xF8, 0x02, 0xFF, 0xF0, 0x01, 0x1A, 0x00, 0x00, 0x00, 0x00 },
		{ 0 }
	},
		/* CORE_SET_CONFIG */
	{
		{ 49, 0x20, 0x02, 0x2E, 0x0C, 0x00, 0x02, 0x2C, 0x01, 0x32, 0x01, 0x20, 0x50, 0x01, 0x00, 0x51, 0x08, 0x20, 0x79, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x53, 0x02, 0x00, 0x01, 0x55, 0x01, 0x01, 0x58, 0x01, 0x07, 0xA0, 0x01, 0x01, 0xD0, 0x01, 0x1E, 0xD1, 0x01, 0x19, 0xD2, 0x01, 0xFF, 0xD3, 0x01, 0x00 },
		{ 0 },
		{ 0 },
	},
		/* RF_DISCOVER_MAP */
	{
		{ 10, 0x21, 0x00, 0x07, 0x02, 0x04, 0x03, 0x02, 0x05, 0x03, 0x03 },
		{ 0 },
		{ 0 },
	},
		/* NFCEE_DISCOVER_CMD Enable*/
	{
		.cmd = { 4, 0x22, 0x00, 0x01, 0x01 },
		.exp_resp_content = { 1, 0x00 },
		.exp_ntf = 	{ 1, 0x01 }
	}
};

static control_msg_pack felica_ese_idm[] ={
	{
		/* CORE_RESET_CMD */
		.cmd = { 4, 0x20, 0x00, 0x01, 0x01 },
		.exp_resp_content = { 6, 0x40, 0x00, 0x03, 0x00, 0x10, 0x01 },
		.exp_ntf = { 0 },
	},
	{
		/* CORE_INIT_CMD */
		{ 3, 0x20, 0x01, 0x00 },
		{ 24, 0x40, 0x01, 0x15, 0x00, 0x01, 0x0E, 0x02, 0x00, 0x04, 0x00, 0x01, 0x02, 0x03, 0x03, 0xF8, 0x02, 0xFF, 0xF0, 0x01, 0x1A, 0x00, 0x00, 0x00, 0x00 },
		{ 0 }
	},
	{
		/* NFCEE_DISCOVER */
		{ 4, 0x22, 0x00, 0x01, 0x01 },
		{ 1, 0x00 },
		{ 1, 0x01 }
	},
	{
		/* NFCEE_MODE_SET_CMD (SAM) */
		{ 5, 0x22, 0x01, 0x02, 0x02, 0x01 },
		{ 1, 0x00 },
		{ 1, 0x01 }
	},
	{
		/* CORE_CONN_CREATE */
		{ 9, 0x20, 0x04, 0x06, 0x03, 0x01, 0x01, 0x02, 0x02, 0x02 },
		{ 1, 0x00 },
		{ 0 }
	},
	{	/* HOST_SAM COMMUNICATION */
		{ 9, 0x02, 0x00, 0x06, 0x06, 0x00, 0xFF, 0xFF, 0x00, 0x00 },
		{ 1, 0x00 },
		{ 1, 0x01 }
	},
};

static eeprom_package nfc_eeprom0_script[] = {
	/* version 06, bank-00 */
	{
		.cmd = {0x2F, 0x38, 0x09, 0x00, 0x00, 0x00, 0x00, 0x04, 0x53, 0x52, 0x47, 0x64},
	},
	{
		{0x2F, 0x38, 0x09, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{
		{0x2F, 0x38, 0x09, 0x08, 0x00, 0x00, 0x00, 0x04, 0x02, 0x24, 0xA0, 0x00},
	},
	{
		{0x2F, 0x38, 0x09, 0x0C, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x40, 0x5E},
	},
	{	
		{0x2F, 0x38, 0x09, 0x10, 0x00, 0x00, 0x00, 0x04, 0x06, 0x24, 0x20, 0x50},
	},
	{
		{0x2F, 0x38, 0x09, 0x14, 0x00, 0x00, 0x00, 0x04, 0x53, 0x35, 0x00, 0x19},
	},
	{	
		{0x2F, 0x38, 0x09, 0x18, 0x00, 0x00, 0x00, 0x04, 0x4C, 0xCC, 0x04, 0x00},
	},
	{
		{0x2F, 0x38, 0x09, 0x1C, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x20, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x24, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x1C, 0x02},
	},
	{		
		{0x2F, 0x38, 0x09, 0x28, 0x00, 0x00, 0x00, 0x04, 0x00, 0xF4, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x2C, 0x00, 0x00, 0x00, 0x04, 0x02, 0x0C, 0x07, 0x01},
	},
	{		
		{0x2F, 0x38, 0x09, 0x30, 0x00, 0x00, 0x00, 0x04, 0x00, 0x10, 0x18, 0x1F},
	},
	{		
		{0x2F, 0x38, 0x09, 0x34, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x10, 0x1F},
	},
	{		
		{0x2F, 0x38, 0x09, 0x38, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01},
	},
	{		
		{0x2F, 0x38, 0x09, 0x3C, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x40, 0x00, 0x00, 0x00, 0x04, 0x1F, 0x1F, 0x1F, 0x1F},
	},
	{		
		{0x2F, 0x38, 0x09, 0x44, 0x00, 0x00, 0x00, 0x04, 0x1F, 0x08, 0x10, 0x02},
	},
	{		
		{0x2F, 0x38, 0x09, 0x48, 0x00, 0x00, 0x00, 0x04, 0x02, 0x04, 0x1F, 0x01},
	},
	{		
		{0x2F, 0x38, 0x09, 0x4C, 0x00, 0x00, 0x00, 0x04, 0x1F, 0x1F, 0x1F, 0x1F},
	},
	{
		{0x2F, 0x38, 0x09, 0x50, 0x00, 0x00, 0x00, 0x04, 0x1F, 0x1F, 0x1F, 0x10},
	},
	{		
		{0x2F, 0x38, 0x09, 0x54, 0x00, 0x00, 0x00, 0x04, 0x10, 0x00, 0x01, 0x01},
	},
	{
		{0x2F, 0x38, 0x09, 0x58, 0x00, 0x00, 0x00, 0x04, 0x08, 0x18, 0x18, 0x10},
	},
	{
		{0x2F, 0x38, 0x09, 0x5C, 0x00, 0x00, 0x00, 0x04, 0x08, 0x18, 0x18, 0x00},
	},
	{
		{0x2F, 0x38, 0x09, 0x60, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x13},
	},
	{
		{0x2F, 0x38, 0x09, 0x64, 0x00, 0x00, 0x00, 0x04, 0x9A, 0x40, 0x1F, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x68, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x02, 0x3A},
	},
	{		
		{0x2F, 0x38, 0x09, 0x6C, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x70, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08},
	},
	{		
		{0x2F, 0x38, 0x09, 0x74, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x78, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02},
	},
	{		
		{0x2F, 0x38, 0x09, 0x7C, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02},
	},
	{		
		{0x2F, 0x38, 0x09, 0x80, 0x00, 0x00, 0x00, 0x04, 0x80, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x84, 0x00, 0x00, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x88, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x20, 0x21},
	},
	{		
		{0x2F, 0x38, 0x09, 0x8C, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xD0},
	},
	{		
		{0x2F, 0x38, 0x09, 0x90, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x20, 0x22},
	},
	{		
		{0x2F, 0x38, 0x09, 0x94, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xD0},
	},
	{		
		{0x2F, 0x38, 0x09, 0x98, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x36, 0x22},
	},
	{		
		{0x2F, 0x38, 0x09, 0x9C, 0x00, 0x00, 0x00, 0x04, 0x0D, 0x0D, 0x13, 0x1A},
	},
	{		
		{0x2F, 0x38, 0x09, 0xA0, 0x00, 0x00, 0x00, 0x04, 0x03, 0x03, 0x05, 0x03},
	},
	{		
		{0x2F, 0x38, 0x09, 0xA4, 0x00, 0x00, 0x00, 0x04, 0x08, 0x88, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xA8, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x60, 0x02},
	},
	{		
		{0x2F, 0x38, 0x09, 0xAC, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x80, 0x20},
	},
	{		
		{0x2F, 0x38, 0x09, 0xB0, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01},
	},
	{		
		{0x2F, 0x38, 0x09, 0xB4, 0x00, 0x00, 0x00, 0x04, 0x1F, 0xFF, 0xC8, 0x10},
	},
	{		
		{0x2F, 0x38, 0x09, 0xB8, 0x00, 0x00, 0x00, 0x04, 0x37, 0xC9, 0x01, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xBC, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x40, 0x7E},
	},
	{		
		{0x2F, 0x38, 0x09, 0xC0, 0x00, 0x00, 0x00, 0x04, 0x1F, 0x33, 0x5B, 0x29},
	},
	{		
		{0x2F, 0x38, 0x09, 0xC4, 0x00, 0x00, 0x00, 0x04, 0x51, 0x65, 0x0D, 0x5E},
	},
	{		
		{0x2F, 0x38, 0x09, 0xC8, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x3C},
	},
	{		
		{0x2F, 0x38, 0x09, 0xCC, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xD0, 0x00, 0x00, 0x00, 0x04, 0x40, 0x00, 0x00, 0x1F},
	},
	{		
		{0x2F, 0x38, 0x09, 0xD4, 0x00, 0x00, 0x00, 0x04, 0x00, 0x02, 0x66, 0x0A},
	},
	{		
		{0x2F, 0x38, 0x09, 0xD8, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x4A, 0x14},
	},
	{		
		{0x2F, 0x38, 0x09, 0xDC, 0x00, 0x00, 0x00, 0x04, 0xFF, 0x00, 0x66, 0xFF},
	},
	{		
		{0x2F, 0x38, 0x09, 0xE0, 0x00, 0x00, 0x00, 0x04, 0x00, 0x3B, 0xFF, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xE4, 0x00, 0x00, 0x00, 0x04, 0x50, 0x66, 0x00, 0xF0},
	},
	{		
		{0x2F, 0x38, 0x09, 0xE8, 0x00, 0x00, 0x00, 0x04, 0x02, 0x82, 0xD4, 0x40},
	},
	{		
		{0x2F, 0x38, 0x09, 0xEC, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xF0, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xF4, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xF8, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xFC, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
};


static eeprom_package nfc_eeprom1_script[] = {
	/* version v7, bank-01 */
	{		
		{0x2F, 0x38, 0x09, 0x00, 0x01, 0x00, 0x00, 0x04, 0x41, 0x4E, 0x59, 0x64},
	},
	{		
		{0x2F, 0x38, 0x09, 0x04, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08},
	},
	{		
		{0x2F, 0x38, 0x09, 0x08, 0x01, 0x00, 0x00, 0x04, 0x03, 0x04, 0x00, 0x80},
	},
	{		
		{0x2F, 0x38, 0x09, 0x0C, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x07, 0x03},
	},
	{		
		{0x2F, 0x38, 0x09, 0x10, 0x01, 0x00, 0x00, 0x04, 0x03, 0x04, 0x00, 0x88},
	},
	{		
		{0x2F, 0x38, 0x09, 0x14, 0x01, 0x00, 0x00, 0x04, 0x00, 0x0E, 0x2A, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x18, 0x01, 0x00, 0x00, 0x04, 0x03, 0x04, 0x00, 0x74},
	},
	{		
		{0x2F, 0x38, 0x09, 0x1C, 0x01, 0x00, 0x00, 0x04, 0x00, 0x33, 0x11, 0x11},
	},
	{		
		{0x2F, 0x38, 0x09, 0x20, 0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x74, 0xF4},
	},
	{		
		{0x2F, 0x38, 0x09, 0x24, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01},
	},
	{		
		{0x2F, 0x38, 0x09, 0x28, 0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x75, 0x40},
	},
	{		
		{0x2F, 0x38, 0x09, 0x2C, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x0B, 0xB8},
	},
	{		
		{0x2F, 0x38, 0x09, 0x30, 0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x75, 0x44},
	},
	{		
		{0x2F, 0x38, 0x09, 0x34, 0x01, 0x00, 0x00, 0x04, 0x1C, 0x1C, 0x04, 0x1C},
	},
	{		
		{0x2F, 0x38, 0x09, 0x38, 0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x75, 0x48},
	},
	{		
		{0x2F, 0x38, 0x09, 0x3C, 0x01, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x01},
	},
	{		
		{0x2F, 0x38, 0x09, 0x40, 0x01, 0x00, 0x00, 0x04, 0x02, 0x05, 0x00, 0x20},
	},
	{		
		{0x2F, 0x38, 0x09, 0x44, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x23},
	},
	{		
		{0x2F, 0x38, 0x09, 0x48, 0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x75, 0x7C},
	},
	{		
		{0x2F, 0x38, 0x09, 0x4C, 0x01, 0x00, 0x00, 0x04, 0x02, 0x00, 0x01, 0x03},
	},
	{		
		{0x2F, 0x38, 0x09, 0x50, 0x01, 0x00, 0x00, 0x04, 0x74, 0x65, 0x72, 0x6D},
	},
	{		
		{0x2F, 0x38, 0x09, 0x54, 0x01, 0x00, 0x00, 0x04, 0x74, 0x65, 0x72, 0x6D},
	},
	{		
		{0x2F, 0x38, 0x09, 0x58, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x5C, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x60, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x64, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x68, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x6C, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x70, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x74, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x78, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x7C, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x80, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x84, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x88, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x8C, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x90, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x94, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x98, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0x9C, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xA0, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xA4, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xA8, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xAC, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xB0, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xB4, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xB8, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xBC, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xC0, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xC4, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xC8, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xCC, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xD0, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xD4, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xD8, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xDC, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xE0, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xE4, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xE8, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xEC, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xF0, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xF4, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xF8, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	},
	{		
		{0x2F, 0x38, 0x09, 0xFC, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00},
	}
};
