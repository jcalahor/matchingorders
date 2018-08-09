Basic sample of how orders are matched in an exchange

Overview of the program:

    * Accept/remove orders as they are entered and keep a book of
      resting orders
      
    * Determine if an accepted order would be satisfied by previously
      accepted orders (i.e. a buy would cross a resting sell)
      
    * Output (print) crossing events and remove completed (fully filled)
      orders from the book
      

Inputs:

    A string of space separated values representing an action.  The number of
    values is determined by the action to be performed and have the following
    format:

    ACTION [OID [SYMBOL SIDE QTY PX]]
    

    ACTION: single character value with the following definitions
    
    O - place order, requires OID, SYMBOL, SIDE, QTY, PX
    
    X - cancel order, requires OID
    
    P - print sorted book (see example below)
    

    OID: positive 32-bit integer value which must be unique for all orders
    

    SYMBOL: alpha-numeric string value. Maximum length of 8.
    

    SIDE: single character value with the following definitions
    
    B - buy
    S - sell

    QTY: positive 16-bit integer value

    PX: positive double precision value (7.5 format)

Outputs:
    A list of strings of space separated values that show the result of the
    action (if any).  The number of values is determined by the result type and
    have the following format:

    RESULT OID [SYMBOL SIDE (FILL_QTY | OPEN_QTY) (FILL_PX | ORD_PX)]

    RESULT: single character value with the following definitions
    F - fill (or partial fill), requires OID, SYMBOL, FILL_QTY, FILL_PX
    X - cancel confirmation, requires OID
    P - book entry, requires OID, SYMBOL, SIDE, OPEN_QTY, ORD_PX (see example below)
    E - error, requires OID. Remainder of line represents string value description of the error

    FILL_QTY: positive 16-bit integer value representing qty of the order filled by
              this crossing event

    OPEN_QTY: positive 16-bit integer value representing qty of the order not yet filled

    FILL_PX:  positive double precision value representing price of the fill of this
              order by this crossing event (7.5 format)

    ORD_PX:   positive double precision value representing original price of the order (7.5 format)

Notes:
    * The implementation is a standalone Linux console application 
    * The app respond to malformed input and other errors with a RESULT
      of type 'E' and a descriptive error message
	  * All orders are standard limit orders
    * Orders are  selected for crossing using price-time (FIFO) priority
    * Orders for different symbols should do not cross and the book supports multiple symbols

Example session:
    INPUT                                   | OUTPUT
    ============================================================================
    "O 10000 IBM B 10 100.00000"            | results.size() == 0
    
    "O 10001 IBM B 10 99.00000"             | results.size() == 0
    
    "O 10002 IBM S 5 101.00000"             | results.size() == 0
    
    "O 10003 IBM S 5 100.00000"             | results.size() == 2
    
                                            | results[0] == "F 10003 IBM 5 100.00000"
					    
                                            | results[1] == "F 10000 IBM 5 100.00000"
					    
    "O 10004 IBM S 5 100.00000"             | results.size() == 2
    
                                            | results[0] == "F 10004 IBM 5 100.00000"
					    
                                            | results[1] == "F 10000 IBM 5 100.00000"
					    
    "X 10002"                               | results.size() == 1
    
                                            | results[0] == "X 10002"
					    
    "O 10005 IBM B 10 99.00000"             | results.size() == 0
    
    "O 10006 IBM B 10 100.00000"            | results.size() == 0
    
    "O 10007 IBM S 10 101.00000"            | results.size() == 0
    
    "O 10008 IBM S 10 102.00000"            | results.size() == 0
    
    "O 10008 IBM S 10 102.00000"            | results.size() == 1
    
                                            | results[0] == "E 10008 Duplicate order id"
					    
    "O 10009 IBM S 10 102.00000"            | results.size() == 0
    
    "P"                                     | results.size() == 6
    
                                            | results[0] == "P 10009 IBM S 10 102.00000"
					    
                                            | results[1] == "P 10008 IBM S 10 102.00000"
					    
                                            | results[2] == "P 10007 IBM S 10 101.00000"
					    
                                            | results[3] == "P 10006 IBM B 10 100.00000"
					    
                                            | results[4] == "P 10001 IBM B 10 99.00000"
					    
                                            | results[5] == "P 10005 IBM B 10 99.00000"
					    
*/
