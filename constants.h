
#ifndef ZADANIE_2_CONSTANTS_H
#define ZADANIE_2_CONSTANTS_H

#include <string>

/* runtime flags */
#define HOST_DEFINED     (1 << 0) /* -h host */
#define RESOURCE_DEFINED (1 << 1) /* -r resource */
#define PORT_DEFINED     (1 << 2) /* -p port */
#define METADATA_DEFINED (1 << 3) /* -m yes|no (optional) */
#define TIMEOUT_DEFINED  (1 << 4) /* -t timeout (optional) */


const std::string OK_response1 = "ICY 200 OK";
const std::string OK_response2 = "HTTP/1.0 200 OK";
const std::string OK_response3 = "HTTP/1.1 200 OK";
const std::string ICY_METAINT = "icy-metaint";
const size_t CONTINUOUS_BUFFER = (2 << 11);
const int TTL_VALUE = 4;

#endif //ZADANIE_2_CONSTANTS_H
