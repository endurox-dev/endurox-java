#!/bin/bash


SCOPE=Nstd

#for f in  BERFU0     BALIGNERR  BNOTFLD    BNOSPACE   BNOTPRES   BBADFLD    BTYPERR    BEUNIX     BBADNAME   BMALLOC    BSYNTAX    BFTOPEN    BFTSYNTAX  BEINVAL    BERFU1     BBADTBL    BBADVIEW   BVFSYNTAX  BVFOPEN    BBADACM    BNOCNAME   BEBADOP    ; do


#for f in  TPEABORT       TPEBADDESC      TPEBLOCK        TPEINVAL        TPELIMIT        TPENOENT        TPEOS           TPEPERM                 TPEPROTO        TPESVCERR       TPESVCFAIL      TPESYSTEM       TPETIME                 TPETRAN                 TPGOTSIG        TPERMERR        TPEITYPE        TPEOTYPE        TPERELEASE      TPEHAZARD       TPEHEURISTIC    TPEEVENT        TPEMATCH        TPEDIAGNOSTIC TPEMIB            TPERFU26      TPERFU27  TPERFU28        TPERFU29        TPINITFAIL     ; do 


for f in  NEINVALINI     NEMALLOC       NEUNIX         NEINVAL        NESYSTEM       NEMANDATORY    NEFORMAT       NETOUT         NENOCONN       NELIMIT        NEPLUGIN       NENOSPACE      NEINVALKEY ; do

cat << EOF
        case AtmiConstants.$f:
            return new ${SCOPE}${f}Exception(msg);

EOF

cat << EOF > ${SCOPE}${f}Exception.java
package org.endurox;

/**
 * $f exception
 */
public class ${SCOPE}${f}Exception extends ${SCOPE}Exception {
        
    public ${SCOPE}${f}Exception(String msg) {
        super(AtmiConstants.${f}, msg);
    }
    
    public ${SCOPE}${f}Exception(String msg, Throwable throwable) {
        super(AtmiConstants.${f}, msg, throwable);
    }
}


EOF

done

