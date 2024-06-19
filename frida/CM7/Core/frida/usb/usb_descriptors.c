#include "TinyUSB/tusb.h"

/* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
 * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *   [MSB]       NET | VENDOR | MIDI | HID | MSC | CDC          [LSB]
 */
#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) | _PID_MAP(ECM_RNDIS, 5) | _PID_MAP(NCM, 5) )

#define USB_VID   0xCafe
#define USB_BCD   0x0200

// String Descriptor Index
enum
{
  STRID_LANGID = 0,
  STRID_MANUFACTURER,
  STRID_PRODUCT,
  STRID_SERIAL,
  STRID_CDC,
  STRID_MSC,
  STRID_INTERFACE,
  STRID_MAC,
};

enum
{
	ITF_NUM_NET,
	ITF_NUM_NET_DATA,
	ITF_NUM_CDC,
	ITF_NUM_CDC_DATA,
	ITF_NUM_MSC,

	ITF_NUM_TOTAL
};

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,

    // Use Interface Association Descriptor (IAD) device class
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,

    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0xCafe,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,

    .iManufacturer      = STRID_MANUFACTURER,
    .iProduct           = STRID_PRODUCT,
    .iSerialNumber      = STRID_SERIAL,

    .bNumConfigurations = 1
};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const * tud_descriptor_device_cb(void)
{
  return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+
#define MAIN_CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_CDC_NCM_DESC_LEN + TUD_CDC_DESC_LEN + TUD_MSC_DESC_LEN)

#define EPNUM_NET_NOTIF   0x81
#define EPNUM_NET_OUT     0x02
#define EPNUM_NET_IN      0x82

#define EPNUM_CDC_NOTIF   0x83
#define EPNUM_CDC_OUT     0x04
#define EPNUM_CDC_IN      0x84

#define EPNUM_MSC_OUT     0x05
#define EPNUM_MSC_IN      0x85

static uint8_t const configuration[] =
{
  // Config number (index+1), interface count, string index, total length, attribute, current in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, MAIN_CONFIG_TOTAL_LEN, 0, 100),

  // Interface number, description string index, MAC address string index, EP notification address and size, EP data address (out, in), and size, max segment size.
  TUD_CDC_NCM_DESCRIPTOR(ITF_NUM_NET, STRID_INTERFACE, STRID_MAC, EPNUM_NET_NOTIF, 64, EPNUM_NET_OUT, EPNUM_NET_IN, CFG_TUD_NET_ENDPOINT_SIZE, CFG_TUD_NET_MTU),
  // Interface number, string index, EP notification address and size, EP data address (out, in) and size.
//  TUD_RNDIS_DESCRIPTOR(ITF_NUM_NET, STRID_INTERFACE, EPNUM_NET_NOTIF, 8, EPNUM_NET_OUT, EPNUM_NET_IN, CFG_TUD_NET_ENDPOINT_SIZE),

  // Interface number, string index, EP notification address and size, EP data address (out, in) and size.
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, STRID_CDC, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, CFG_TUD_CDC_EP_BUFSIZE),

  // Interface number, string index, EP Out & EP In address, EP size
  TUD_MSC_DESCRIPTOR(ITF_NUM_MSC, STRID_MSC, EPNUM_MSC_OUT, EPNUM_MSC_IN, CFG_TUD_MSC_EP_BUFSIZE),

};


// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
  return configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// array of pointer to string descriptors
static char const* string_desc_arr [] =
{
  [STRID_LANGID]       	= (const char[]) { 0x09, 0x04 }, 	// supported language is English (0x0409)
  [STRID_MANUFACTURER] 	= "PSI",                         	// Manufacturer
  [STRID_PRODUCT]      	= "Frida",          				// Product
  [STRID_SERIAL]       	= NULL,                      		// Serial
  [STRID_INTERFACE]    	= "Frida Network Interface",   		// Interface Description

  // STRID_MAC index is handled separately
  [STRID_CDC]    		= "Frida CDC",   				 	// CDC Interface
  [STRID_MSC]    		= "Frida MSC"   				 	// MSC Interface
};

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;
  unsigned int chr_count = 0;

  switch ( index ) {
    case STRID_LANGID:
      memcpy(&_desc_str[1], string_desc_arr[0], 2);
      chr_count = 1;
      break;

    case STRID_SERIAL:
      break;

    case STRID_MAC:
      // Convert MAC address into UTF-16
      for (unsigned i=0; i<sizeof(tud_network_mac_address); i++) {
        _desc_str[1+chr_count++] = "0123456789ABCDEF"[(tud_network_mac_address[i] >> 4) & 0xf];
        _desc_str[1+chr_count++] = "0123456789ABCDEF"[(tud_network_mac_address[i] >> 0) & 0xf];
      }
      break;

    default:
      // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
      // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

      if ( !(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) ) return NULL;

      const char *str = string_desc_arr[index];

      // Cap at max char
      chr_count = strlen(str);
      size_t const max_count = sizeof(_desc_str) / sizeof(_desc_str[0]) - 1; // -1 for string type
      if ( chr_count > max_count ) chr_count = max_count;

      // Convert ASCII string into UTF-16
      for ( size_t i = 0; i < chr_count; i++ ) {
        _desc_str[1 + i] = str[i];
      }
      break;
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8 ) | (2*chr_count + 2));

  return _desc_str;
}
