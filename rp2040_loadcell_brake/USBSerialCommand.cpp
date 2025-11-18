// based on https://github.com/argandas/SerialCommand

#include "USBSerialCommand.h"



#if (SERIAL_CMD_DBG_EN == 1)

static void printHex(Stream &port, uint8_t *data, uint8_t length);
static void printHex(Stream &port, uint16_t *data, uint8_t length);

void printHex(Stream &port, uint8_t *data, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
    for (int i = 0; i < length; i++)
    {
        // ort.print("0x");
        if (data[i] < 0x10)
        {
            port.print("0");
        }
        port.print(data[i], HEX);
        port.print(" ");
    }
}

void printHex(Stream &port, uint16_t *data, uint8_t length) // prints 16-bit data in hex with leading zeroes
{
    for (int i = 0; i < length; i++)
    {
        // port.print("0x");
        uint8_t MSB = byte(data[i] >> 8);
        uint8_t LSB = byte(data[i]);
        if (MSB < 0x10)
        {
            port.print("0");
        }
        port.print(MSB, HEX);
        if (LSB < 0x10)
        {
            port.print("0");
        }
        port.print(LSB, HEX);
        port.print(" ");
    }
}
#endif

SerialCommand::SerialCommand() : userErrorHandler(NULL), commandCount(0)
{
    clear();
}

void SerialCommand::begin()
{

}

// This checks the Serial stream for characters, and assembles them into a buffer.
// When the terminator character (defined by EOL constant) is seen, it starts parsing the
// buffer for a prefix command, and calls handlers setup by addCommand() method
void SerialCommand::loop(void)
{
    char c;
    while (available() > 0)
    {
        c = read();
        bufferHandler(c);
    }

    // usb_serial.print(buffer);
}

/* Clear buffer */
void SerialCommand::clear(void)
{
    memset(buffer, 0, SERIAL_CMD_BUFF_LEN);
    pBuff = buffer;
}

/* 
 * Send error response
 * NOTE: Will execute user defined callback (defined using addDefault method),
 * if no user defined callback it will send the ERROR message (sendERROR method).
 */
void SerialCommand::error(void)
{
    if (NULL != userErrorHandler)
    {
        (*userErrorHandler)();
    }

    clear(); /* Clear buffer */
}

// Retrieve the next token ("word" or "argument") from the Command buffer.
// returns a NULL if no more tokens exist.
char *SerialCommand::next(void)
{
    return strtok_r(NULL, delimiters, &last);
}

void SerialCommand::bufferHandler(char c)
{
    int len;
    char *lastChars = NULL;

    if ((pBuff - buffer) > (SERIAL_CMD_BUFF_LEN - 2)) /* Check buffer overflow */
    {
        error(); /* Send ERROR, Buffer overflow */
    }

    *pBuff++ = c;  /* Put character into buffer */
    *pBuff = '\0'; /* Always null terminate strings */

    if ((pBuff - buffer) > 2) /* Check buffer length */
    {
        /* Get EOL */
        len = strlen(buffer);
        lastChars = buffer + len - 2;

        /* Compare last chars to EOL */
        if (0 == strcmp(lastChars, EOL))
        {

            // *lastChars = '\0'; /* Replace EOL with NULL terminator */

#if (SERIAL_CMD_DBG_EN == 1)
            print("Received: ");
            println(buffer);
#endif

            if (commandHandler())
            {
                clear();
            }
            else
            {
                error();
            }
        }
    }
}

/* Return true if match was found */
bool SerialCommand::commandHandler(void)
{
    int i;
    bool ret = false;
    char *token = NULL;
    char *offset = NULL;
    char userInput[SERIAL_CMD_BUFF_LEN];

    memcpy(userInput, buffer, SERIAL_CMD_BUFF_LEN);

    /* Search for command at start of buffer */
    token = strtok_r(buffer, delimiters, &last);

#if (SERIAL_CMD_DBG_EN == 1)
    print("User input: (");
    printHex(Serial, (uint8_t *)userInput, SERIAL_CMD_BUFF_LEN);
    println(")");
#endif

    if (NULL != token)
    {

#if (SERIAL_CMD_DBG_EN == 1)
        print("Token: \"");
        print(token);
        println("\"");
#endif

        for (i = 0; (i < commandCount); i++)
        {          
            if (!strncmp(token, commandList[i].command, SERIAL_CMD_BUFF_LEN))
            {
              (*commandList[i].execute)();
              return true;
            }
        }
    }

    return ret;
}

// Adds a "command" and a handler function to the list of available commands.
// This is used for matching a found token in the buffer, and gives the pointer
// to the handler function to deal with it.
void SerialCommand::addCommand(char *cmd, void (*test)(), void (*read)(), void (*write)(), void (*execute)())
{

#if (SERIAL_CMD_DBG_EN == 1)
    print("[");
    print(commandCount);
    print("] New Command: ");
    println(cmd);
#endif

    commandList = (serialCommandCallback *)realloc(commandList, (commandCount + 1) * sizeof(serialCommandCallback));
    strncpy(commandList[commandCount].command, cmd, SERIAL_CMD_BUFF_LEN);
    commandList[commandCount].test = test;
    commandList[commandCount].read = read;
    commandList[commandCount].write = write;
    commandList[commandCount].execute = execute;
    commandCount++;
}

/* Optional user-defined function to call when an error occurs, default is NULL */
void SerialCommand::addError(void (*callback)())
{
    userErrorHandler = callback;
}

int SerialCommand::available()
{
    int bytes = 0;
    bytes = usb_serial.available();
    return bytes;
}

int SerialCommand::read()
{
    int bytes = 0;
    bytes = usb_serial.read();    
    return bytes;
}

int SerialCommand::peek()
{
    int bytes = 0;    
    bytes = usb_serial.peek();    
    return bytes;
}

void SerialCommand::flush()
{
    usb_serial.flush();
}

size_t SerialCommand::write(uint8_t x)
{
    (void)x;
    return 0;
}

void SerialCommand::printHelp()
{
    for(int i = 0 ; i <commandCount ; i++)
    {
        usb_serial.printf("%s\n", commandList[i].command);
    }
}

float SerialCommand::nextFloat()
{
    auto s = next();
    return atof(s);
}

int SerialCommand::nextInt()
{
    auto s = next();
    Con_Printf("s=%s\n",s);
    return atoi(s);
}
