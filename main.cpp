# include <iostream>
# include <string>
# include <vector>
# include <fstream>

using namespace std ;

# define SPACE ' '
# define TAB '  '
# define ENTER '\n'
# define COMMENT '.'

// used to locate the table
# define INSTR 1
# define PSEUDO 2
# define REG 3
# define DELIMITER 4
# define SYMBOL 5
# define INT 6
# define STR 7

# define tableStartNo 1
# define tableEndNo 4

vector<string> instrSet, pseudoSet, regSet, delimiterSet ;

struct Token {
    string str ; // the origin string of the token
    int table ; // the table which this token belong with
    int no ; // the no. of this token in this table
} ;

struct originInstr {
    int lineNo ;
    string str ;
    int numOfToken ;
    vector<Token> tokens ; // store the desperate token
} ;

struct tableInfo {
    int no ; // no. of the table
    string str ;
} ;

class Tool {
    
    public :
    
    void setTableContent( int tableNo, vector<tableInfo> tableInFile ) {
        
        if ( tableNo == 1 ) {
            
            for ( int i = 0 ; i < tableInFile.size() ; i ++ ) {
                
                instrSet.push_back( upperToLower( tableInFile[ i ].str ) ) ;
                
            } // for()
            
        } // if()
        else if ( tableNo == 2 ) {
            
            for ( int i = 0 ; i < tableInFile.size() ; i ++ ) {
                
                pseudoSet.push_back( upperToLower( tableInFile[ i ].str ) ) ;
                
            } // for()
            
        } // else if()
        else if ( tableNo == 3 ) {
            
            for ( int i = 0 ; i < tableInFile.size() ; i ++ ) {
                
                regSet.push_back( upperToLower(  tableInFile[ i ].str ) ) ;
                
            } // for()
            
        } // else if()
        else if ( tableNo == 4 ) {
            
            for ( int i = 0 ; i < tableInFile.size() ; i ++ ) {
                
                delimiterSet.push_back( tableInFile[ i ].str ) ;
                
            } // for()
            
        } // else if()
        
    } //setTableContent()
    
    bool isSpace( char ch ) {
        
        if ( ch == ' ' || ch == '\t' ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isSpace()
    
    bool isINSTR( string str ) {
        
        vector<string> :: iterator result = find( instrSet.begin(), instrSet.end(), upperToLower( str ) ) ;
        
        if ( result != instrSet.end() ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isINSTR()
    
    bool isPSEUDO( string str ) {
        
        vector<string> :: iterator result = find( pseudoSet.begin(), pseudoSet.end(), upperToLower( str ) ) ;
        
        if ( result != pseudoSet.end() ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isPSEUDO()
    
    bool isREG( string str ) {
        
        vector<string> :: iterator result = find( regSet.begin(), regSet.end(), upperToLower( str ) ) ;
        
        if ( result != regSet.end() ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isREG()
    
    bool isDELIMITER( string str ) {
        
        vector<string> :: iterator result = find( delimiterSet.begin(), delimiterSet.end(), upperToLower( str ) ) ;
        
        if ( result != delimiterSet.end() ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isDELIMITER()
    
    bool isINT( string str ) {
        
        // check that each character is between '0' and '9'
        
        if ( ( str[ 0 ] == 'x' || str[ 0 ] == 'X' ) && str[ 1 ] == '\'' && str[ str.length() - 1] == '\'' ) {
            
            return true ;
            
        } // if()
        else {

            for ( int i = 0 ; i < str.length() ; i ++ ) {
                
                if ( str[ i ] > '9' || str[ i ] < '0' ) {
                    
                    return false ;
                    
                } // if()
                
            } // for()
            
        } // else()
        
        return true ;
        
    } // isINT()
    
    bool isSTR( string str ) {
        
        if ( ( str[ 0 ] == 'c' || str[ 0 ] == 'C' ) && str[ 1 ] == '\'' && str[ str.length() - 1] == '\'' ) {
            
            return true ;
            
        } // if()
        
        return false ;
        
    } // isSTR()
    
    int classifyToken( string str ) {
        
        int category = -1 ;
        
        // table1 : instruction
        if ( isINSTR( str ) ) {
            
            category = INSTR ;
            
        } // if()
        // table2 : pseudo instruction
        else if ( isPSEUDO( str ) ) {
            
            category = PSEUDO ;
            
        } // else if()
        // table3 : register
        else if ( isREG( str ) ) {
            
            category = REG ;
            
        } // else if()
        // table4 : delimiter
        else if ( isDELIMITER( str ) ) {
            
            category = DELIMITER ;
            
        } // else if()
        // table7 : string
        else if ( isSTR( str ) ) {
            
            category = STR ;
            
        } // else if()
        // table6 : integer
        else if ( isINT( str ) ) {
            
            category = INT ;
            
        } // else if()
        // table5 : symbol
        else {
            
            category = SYMBOL ;
            
        } // else()
        
        return category ;
        
    } // classifyToken
    
    string itoa( int num ) {
        
        char ch = '\0' ;
        string str = "" ;
        
        ch = '0' + num ;
        
        str += ch ;
        
        return str ;
        
    } // itoa()
    
    int transferToAscii( string str ) {
        
        int ans = 0 ;
        
        for ( int i = 0 ; i < str.length() ; i ++ ) {
            
            ans += str[ i ] ;
            
        } // for()
        
        return ans ;
        
    } // transferToAscii()
    
    void skipSpace( int &index, string str ) {
        
        while ( isSpace( str[ index ] ) && index < str.length() ) {
            
            index ++ ;
            
        } // while()
        
    } // skipSpace()
    
    vector<Token> splitInstr( string str ) {
        
        vector<Token> tokenList ;
        
        int startIndex = 0 ;
        
        skipSpace( startIndex, str ) ;
        
        // assert : we have avoid the space in front of the first token
        
        while ( startIndex < str.length() ) {
            
            string tmpStr = "" ;
            
            string charToStr = "" ;
            bool stop = false ;
            for ( ; startIndex < str.length() && !isSpace( str[ startIndex ] ) && !stop ; startIndex ++ ) {
                
                charToStr = "" ;
                charToStr = str[ startIndex ] ;
                
                if ( !isDELIMITER( charToStr ) || charToStr == "'" ) {
                    
                    tmpStr += str[ startIndex ] ;
                    
                } // if()
                else {
                    
                    stop = true ;
                    
                } // else()
                
            } // for()
            
            // special case
            if ( ( isSTR( tmpStr ) || isINT( tmpStr ) ) && tmpStr != "" ) {
                
                vector<Token> tmpList = parseString( tmpStr ) ;
                
                for ( int i = 0 ; i < tmpList.size() ; i ++ ) {
                    
                    tokenList.push_back( tmpList[ i ] ) ;
                    
                } // for()
                
                charToStr = "" ;
                
            } // if()
            else if ( tmpStr != "" ) {
                
                // assert : we get one token
                
                Token token ;
                token.str = tmpStr ;
                token.table = -1 ;
                token.no = -1 ;
                tokenList.push_back( token ) ;
                
                skipSpace( startIndex, str ) ;
                
            } // else()
            
            if ( isDELIMITER( charToStr ) && charToStr != "" ) {
                
                Token token ;
                token.str = charToStr ;
                token.table = DELIMITER ;
                token.no = -1 ;
                tokenList.push_back( token ) ;
                
            } // if()
            
        } // while()
        
        return tokenList ;
        
    } // splitInstr()
    
    string upperToLower( string str ) {
        
        string tmpStr = "" ;
        
        for ( int i = 0 ; i < str.length() ; i ++ ) {
            
            if ( str[ i ] >= 'A' && str[ i ] <= 'Z' ) {
                
                tmpStr += str[ i ] + 32 ;
                
            } // if()
            else {
                
                tmpStr += str[ i ] ;
                
            } // else()
            
        } // for()
        
        return tmpStr ;
        
    } // upperToLower()
    
    string lowerToUp( string str ) {
        
        string tmpStr = "" ;
        
        for ( int i = 0 ; i < str.length() ; i ++ ) {
            
            if ( str[ i ] >= 'a' && str[ i ] <= 'z' ) {
                
                tmpStr += str[ i ] - 32 ;
                
            } // if()
            else {
                
                tmpStr += str[ i ] ;
                
            } // else()
            
        } // for()
        
        return tmpStr ;
        
    } // upperToLower()
    
    vector<Token> parseString( string str ) {
        
        // three types : 1. c'EOF' 2. x'ABC'
        
        vector<Token> tmpList ;
        
        string tmpStr = "" ;
        
        if ( isSTR( str ) ) {
            
            for ( int i = 1 ; i < str.length() ; i ++ ) {
                
                Token tmpToken ;
                
                string charToStr = "" ;
                
                charToStr += str[ i ] ;
                
                if ( isDELIMITER( charToStr ) && charToStr != "'" ) {
                    
                    tmpToken.str = charToStr ;
                    tmpToken.table = DELIMITER ;
                    tmpToken.no = -1 ;
                    
                    tmpList.push_back( tmpToken ) ;
                    
                } // if()
                else if ( str[ i ] == '\'' ) {
                    
                    if ( i != 1 ) {
                        
                        tmpToken.str = tmpStr ;
                        tmpToken.table = STR ;
                        tmpToken.no = -1 ;
                        
                        tmpList.push_back( tmpToken ) ;
                        
                    } // if()
                    
                    tmpToken.str = charToStr ;
                    tmpToken.table = DELIMITER ;
                    tmpToken.no = -1 ;
                    
                    tmpList.push_back( tmpToken ) ;
                    
                } // else if()
                else {
                    
                    tmpStr += str[ i ] ;
                    
                } // else()
                
            } // for()
            
        } // if()
        else if ( isINT( str ) ) {
            
            string tmpStr = "" ;
            
            for ( int i = 0 ; i < str.length() ; i ++ ) {
                
                Token tmpToken ;
                
                if ( ( str[ i ] == 'x' || str[ i ] == 'X' ) && i == 0 ) {
                    
                    i ++ ;
                    
                } // if()
                
                if ( str[ i ] == '\'' ) {
                    
                    if ( i != 1 ) {
                        
                        tmpToken.str = lowerToUp( tmpStr ) ;
                        tmpToken.table = INT ;
                        tmpToken.no = -1 ;
                        
                        tmpList.push_back( tmpToken ) ;
                        
                    } // if()
                    
                    tmpToken.str = str[ i ] ;
                    tmpToken.table = -1 ;
                    tmpToken.no = -1 ;
                    
                    tmpList.push_back( tmpToken ) ;
                    
                } // if()
                else {
                    
                    tmpStr += str[ i ] ;
                    
                    if ( i == str.length() - 1 ) {
                        
                        tmpToken.str = lowerToUp( tmpStr ) ;
                        tmpToken.table = INT ;
                        tmpToken.no = -1 ;
                        
                        tmpList.push_back( tmpToken ) ;
                        
                    } // if()
                    
                } // else()
                
            } // for()
            
        } // else if()
        
        return tmpList ;
        
    } // parseString()
    
} ; // Tool

class ProcessTable {
    
    private :
    
    fstream file ;
    
    Tool tool ;
    
    vector<tableInfo> buffer_table_1to4[ 4 ] ;
    tableInfo buffer_table_5to7[ 100 ][ 3 ] ;
    
    public :
    
    void findTokenInTable( Token token, int &tableNo, int &column ) {
         
        tableNo = -1 ;
        column = -1 ;
        
        string lowerStr = tool.upperToLower( token.str ) ; // transfer the token string into lower case for comparison
        
        if ( token.table != -1 ) {
            
            tableNo = token.table ;
            
        } // if()
        else {
            
            tableNo = tool.classifyToken( lowerStr ) ;
            
        } // else()
         
         if ( tableNo >= 1 && tableNo <= 4 ) {
             
             for ( int i = 0 ; i < buffer_table_1to4[ tableNo - 1 ].size() && column == -1 ; i ++ ) {
                 
                 if ( lowerStr == buffer_table_1to4[ tableNo - 1 ][ i ].str ) { // find the same token
                     
                     column = buffer_table_1to4[ tableNo - 1 ][ i ].no ;
                     
                 } // if()
                 
             } // for()
             
         } // if()
         else {
             
             bool isFind = false ;
             for ( int i = 0 ; i < 100 && !isFind ; i ++ ) {
                 
                 if ( token.str == buffer_table_5to7[ i ][ tableNo - 5 ].str ) {
                     
                     isFind = true ;
                     
                     column = buffer_table_5to7[ i ][ tableNo - 5 ].no ;
                     
                     isFind = true ;
                     
                 } // if()
                 
             } // for()
             
             if ( !isFind ) { // if we doesn't find the token in the table
                 
                 addToTable( tableNo, token.str, column ) ;
                 
             } // if()
             
         } // else()
         
     } // findTokenInTable()
    
    bool readFile() {
        
        string startNo = "" ;
        string endNo = "" ;
        
        bool success = false ;
        do {
            
            cout << endl << "[Please enter START with table no. (Ex.1) ]: " ;
            
            cin >> startNo ;
            
            cout << endl << "[Please enter END with table no. (Ex.4) ]: " ;
            
            cin >> endNo ;
            
            if (  startNo != tool.itoa( tableStartNo ) || endNo != tool.itoa( tableEndNo )  )  {
                
                cout << endl << "### Error: illegal table start or end No.! ###" << endl << endl ;
                
                cout << "Please try again!" << endl ;
                
            } // if()
            else {
                
                success = true ;
                
            } // else()
            
        } while ( !success );
        
        for ( int i = 0 ; i <= atoi( endNo.c_str() ) - atoi( startNo.c_str() ) ; i ++ ) {
            
            file.open( "Table" + tool.itoa( i + 1 ) + ".table", ios :: in ) ;
            
            if ( !file ) {
                
                cout << "### Error: build the table failed! ###" << endl << endl ;
                
                return false ;
                
            } // if()
            
            int countNo = 0 ;
            vector<tableInfo> tmp ;
            while ( !file.eof() ) {
                
                tableInfo tmpInfo ;
                string tokenStr ;
                
                getline( file, tokenStr ) ;
                
                string tmpStr = "" ;
                for ( int i = 0 ; i < tokenStr.length() && tokenStr[ i ] != '\r' && tokenStr[ i ] != ' ' ; i ++ ) {
                    
                    tmpStr += tokenStr[ i ] ;
                    
                } // for()
                tokenStr = tmpStr ;
                
                // table1~table3 we need to transfer the string into lower case
                if ( i < atoi( endNo.c_str() ) - atoi( startNo.c_str() - 1 ) ) {
                    
                    tokenStr = tool.upperToLower( tokenStr ) ;
                    
                } // if()
                
                tmpInfo.no = countNo + 1 ;
                tmpInfo.str = tokenStr ;
                
                countNo ++ ;
                
                tmp.push_back( tmpInfo ) ;
                
            } // while()
            
            // assert : one of the table is finish reading
            
            buffer_table_1to4[ i ] = tmp ; // tables are in order
            
            tool.setTableContent( i + 1, tmp ) ; // we set the information from the data in the tool, then the tool we konw how to determine the token
            
            file.close() ;
            
        } // for()
        
        // initialize table 5 - 7
        for ( int i = 0 ; i < 3 ; i ++ ) {
            
            for ( int j = 0 ; j < 100 ; j ++ ) {
                
                tableInfo initialTable ;
                initialTable.str = "" ;
                initialTable.no = j + 1 ;
                
                buffer_table_5to7[ j ][ i ] = initialTable ;
                
            } // for()
            
        } // for()
        
        return true ;
        
    } // readFile()
    
    bool saveFile() {
        
        for ( int i = 0 ; i < 4 ; i ++ ) {
            
            int no = tool.classifyToken( buffer_table_1to4[ i ][ 0 ].str ) ; // the first token of this table
            
            file.open( "myTable" + tool.itoa( no ) + ".table", ios :: out ) ;
            
            if ( !file ) {
                
                cout << "### Error: fail to save the table into files! ###" << endl << endl ;
                
                return false ;
                
            } // if()
            
            for ( int j = 0 ; j < buffer_table_1to4[ i ].size() ; j ++ ) {
                
                file << buffer_table_1to4[ i ][ j ].str << endl ;
                
            } // for()
            
            cout << "*** Successful! Create [myTable" + tool.itoa( no ) + ".table]" << "! ***" << endl << endl ;
            
            file.close() ;
            
            buffer_table_1to4[ i ].clear() ;
            
        } // for()
        
        for ( int i = 0 ; i < 3 ; i ++ ) {
            
            int no = tool.classifyToken( buffer_table_5to7[ 0 ][ i ].str ) ; // the first token of this table
            
            file.open( "myTable" + tool.itoa( i + 5 ) + ".table", ios :: out ) ;
            
            if ( !file ) {
                
                cout << "### Error: fail to save the table into files! ###" << endl << endl ;
                
                return false ;
                
            } // if()
            
            for ( int j = 0 ; j < 100 ; j ++ ) {
                
                file << buffer_table_5to7[ j ][ i ].str << endl ;
                
            } // for()
            
            cout << "*** Successful! Create [myTable" + tool.itoa( i + 5 ) + ".table]" << "! ***" << endl << endl ;
            
            file.close() ;
            
        } // for()
        
        return true ;
         
    } // saveFile()
    
    void addToTable( int tableNo, string tokenStr, int &columnWePutIn ) {
        
        int ascii = tool.transferToAscii( tokenStr ) ;
        
        int index = ascii % 100 ;
        
        while ( buffer_table_5to7[ index - 1 ][ tableNo - 5 ].str != "" ) { // the column is not empty
            
            index ++ ;
            
            if ( index == 100 ) {
                
                index = 0 ; // start with the begin
                
            } // if()
            
        } // if()
        
        buffer_table_5to7[ index - 1 ][ tableNo - 5 ].str = tokenStr ;
        
        columnWePutIn = index ;
        
    } // addToTable()
    
} ; // ProcessTable

class ProcessData {
    
    private :
    
    fstream file ;
    
    string fileName ;
    
    vector<originInstr> buffer ;
    
    Tool tool ;
    
    ProcessTable processTable ; // most important is to store the tables
    
    void printTokens() {
        
        for ( int i = 0 ; i < buffer.size() ; i ++ ) {
            
            cout << buffer[ i ].str << endl ;
            
            for ( int j = 0 ; j < buffer[ i ].numOfToken ; j ++ ) {
                
                cout << "(" << buffer[ i ].tokens[ j ].table << "," << buffer[ i ].tokens[ j ].no << ")" ;
                
            } // for()
            
            cout << endl ;
            
        } // for()
        
    } // printTokens()
    
    public :
    
    bool readFile() {
        
        string fileStr = "" ;
        
        cout << "[Please Enter the file name (Quit:q) ]: " ;
        
        cin >> fileStr ;
        
        file.open( fileStr + ".txt", ios :: in ) ;
        
        while ( !file ) {
            
            if ( fileStr == "q" ) {
                
                return false ;
                
            } // if()
            else {
                
                cout << endl << "### Error: There is no such file "<< fileStr + ".txt" << "! ###" << endl << endl ;
                
                cout << "Please try again!" << endl << endl ;
                
                cout << "[Please Enter the file name (Quit:q) ]: " ;
                
                cin >> fileStr ;
                
                file.open( fileStr + ".txt", ios :: in ) ;
                
            } // else()
            
        } // while()
        
        // assert : we have successfully open the file that we now can start read the information
        
        fileName = fileStr ;
        
        int lineNumber = 0 ; // count which line in the file right now
        string line = "" ; // the whole instruction we saw in the file
        
        while ( getline( file, line ) ) {
            
            // first we have to deal with the whole string, clean the ENTER and find the comment
            string tmpLine = "" ;
            for ( int i = 0 ; i < line.length() && line[ i ] != '\r' ; i ++ ) {
                
                tmpLine += line[ i ] ;
                
            } // for()
            line = tmpLine ;
            
            originInstr tmp ;
            lineNumber ++ ;
            
            tmp.lineNo = lineNumber ;
            tmp.str = line ;
            
            buffer.push_back( tmp ) ;
            
        } // while()
        
        file.close() ; // close the file
        
        readTable() ;
        
        return true ;
        
    } // readFile()
    
    void readTable() {
        
        processTable.readFile() ;
        
    } // readTable()
    
    void getToken() {
        
        // assert : all the instruction are put in the buffer
        
        for ( int i = 0 ; i < buffer.size() ; i ++ ) {
            
            buffer[ i ].lineNo = i + 1 ;
            buffer[ i ].tokens = tool.splitInstr( buffer[ i ].str ) ;
            buffer[ i ].numOfToken = ( int ) buffer[ i ].tokens.size() ;
            
            for ( int j = 0 ; j < buffer[ i ].numOfToken ; j ++ ) {
                
                processTable.findTokenInTable( buffer[ i ].tokens[ j ], buffer[ i ].tokens[ j ].table, buffer[ i ].tokens[ j ].no ) ;
            } // for()
            
        } // for()
        
        // printTokens() ;
        
    } // getToken()
    
    bool saveFile() {
        
        file.open( "my_" + fileName + "_output.txt", ios :: out ) ;
        
        if ( !file ) {
            
            cout << "### Error: Can't open such file "<< fileName + "_output.txt" << "! ###" << endl << endl ;
            
            return false ;
            
        } // if()
        
        // assert : we successfully open the output file that we now can write data into the file
        
        for ( int i = 0 ; i < buffer.size() ; i ++ ) {
            
            file << buffer[ i ].str << "\n" ; // write the whole instruction first
            
            for ( int j = 0 ; j < buffer[ i ].tokens.size() ; j ++ ) {
                
                file << "(" << buffer[ i ].tokens[ j ].table << "," << buffer[ i ].tokens[ j ].no << ")" ;
                
            } // for()
            
            file << "\n" ;
            
        } // for()
        
        cout << endl << "*** Successful! Create ["<< fileName + "_output.txt] " << "! ***" << endl << endl ;
        
        file.close() ;
        
        processTable.saveFile() ;
        
        return true ;
        
    } // saveFile()
    
    void printBuffer() {
        
        for ( int i = 0 ; i < buffer.size() ; i ++ ) {
            
            cout << buffer[ i ].str << endl << endl ;
            
        } // for()
        
    } // printBuffer()
    
} ; // ProcessData

int main() {
    
    ProcessData process_data ;
    
    if ( process_data.readFile() ) {
        
        // process_data.printBuffer() ;
        process_data.getToken() ;
        process_data.saveFile() ;
        
    } // if()
    
} // main()
