
#ifndef __IP_SETTINGS_H__
#define __IP_SETTINGS_H__

#define UUID ((uint8_t *) UID_BASE)  // Unique 96-bit chip ID. TRM 39.1

// Helper macro for MAC generation
#define GENERATE_LOCALLY_ADMINISTERED_MAC()                        \
  {                                                                \
    2, UUID[0] ^ UUID[1], UUID[2] ^ UUID[3], UUID[4] ^ UUID[5],    \
        UUID[6] ^ UUID[7] ^ UUID[8], UUID[9] ^ UUID[10] ^ UUID[11] \
  }

//#define MAC_ADDRESS   {0x20, 0x89, 0x84, 0x6a, 0x96, 0x00}  // Our default MAC-address
#define MAC_ADDRESS   GENERATE_LOCALLY_ADMINISTERED_MAC()  // Our default MAC-address

#define IP_ADDRESS    IPADDR4_INIT_BYTES(  10,   0,  0 ,   1)
//#define IP_ADDRESS    IPADDR4_INIT_BYTES( 169, 254, 1, 1)

#define IP_NETMASK    IPADDR4_INIT_BYTES( 255, 255, 255,   0)
//#define IP_NETMASK    IPADDR4_INIT_BYTES( 255, 255, 0,   0)

#define IP_GATEWAY    IPADDR4_INIT_BYTES(  10,   0,   0, 100)
//#define IP_GATEWAY    IPADDR4_INIT_BYTES( 169, 254, 254, 100)


#define IPADDR4_INIT_BYTES(a,b,c,d) (((uint32_t)((d) & 0xff) << 24) | \
                                     ((uint32_t)((c) & 0xff) << 16) | \
                                     ((uint32_t)((b) & 0xff) << 8)  | \
                                      (uint32_t)((a) & 0xff))


#endif /* __IP_SETTINGS_H__ */
