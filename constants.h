
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

/* radio-proxy runtime flags */
#define HOST_DEFINED        (1 << 0) /* -h host */
#define RESOURCE_DEFINED    (1 << 1) /* -r resource */
#define PORT_DEFINED        (1 << 2) /* -p port */
#define METADATA_DEFINED    (1 << 3) /* -m yes|no (optional) */
#define TIMEOUT_DEFINED     (1 << 4) /* -t timeout (optional) */
#define UDP_PORT_DEFINED    (1 << 5) /* -P port */
#define UDP_MULTI_DEFINED   (1 << 6) /* -B multi (optional) */
#define UDP_TIMEOUT_DEFINED (1 << 7) /* -T timeout (optional) */

/* radio-client runtime flags */
#define CLIENT_HOST_DEFINED     (1 << 0) /* -H host */
#define CLIENT_UDP_PORT_DEFINED (1 << 1) /* -P port */
#define CLIENT_TCP_PORT_DEFINED (1 << 2) /* -p port */
#define CLIENT_TIMEOUT_DEFINED  (1 << 3) /* -T timeout (optional) */

const std::string OK_response1 = "ICY 200 OK";
const std::string OK_response2 = "HTTP/1.0 200 OK";
const std::string OK_response3 = "HTTP/1.1 200 OK";
const std::string ICY_METAINT  = "icy-metaint";
const std::string ICY_NAME     = "icy-name";
const size_t CONTINUOUS_BUFFER = (1 << 12);
const int TTL_VALUE = 4;
const int BSIZE = (1 << 16) + 4;
const int DATA_LENGTH = 1500;
const int OCTET = (1 << 8);
const long double THREE_AND_HALF_USECONDS = 3.5;

/* datagram types */
const uint16_t DISCOVER  = 1;
const uint16_t IAM       = 2;
const uint16_t KEEPALIVE = 3;
const uint16_t AUDIO     = 4;
const uint16_t METADATA  = 6;

/* poll groups */
#define TCP_POLL 0
#define UDP_POLL 1


#endif //ZADANIE_2_CONSTANTS_H
