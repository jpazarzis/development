#!/usr/bin/python

from math import factorial

def combos(k,n):
    if k < n:
        return 0
    if k == n:
        return 1
    return factorial(k) / ( factorial(k-n) * factorial(n) )

def tokens_to_html_table(tokens):
    lines = ['<table class="altrowstable" id="alternatecolor">']
    lines.append('<col width="30"/>')
    lines.append('<col width="30"/>')
    lines.append('<col width="30"/>')
    lines.append('<col width="30"/>')

    lines.append('<tr>')

    for t in tokens[0]:
        lines.append('<th>')
        lines.append('{0}'.format(t))
        lines.append('</th>')
    lines.append('</th>')

    for line in tokens[1:]:
        lines.append('<tr>')
        for t in line:
            lines.append('<td style="text-align: right;">')
            lines.append('{0}'.format(t))
            lines.append('</td>')
        lines.append('</tr>')
    lines.append('</table>')

    return '\n'.join(lines)

def show_behavior(k,n):

    print "<span style='float:left;border:1px solid blue;margin-left:5px;margin-bottom:25px;background-color:#ddd;'>"
    print 'Parlay Length:<b>',n,'</b><br/>Combos: <b>{0:6}</b>'.format(combos(k,n))
    cost = combos(k,n)
    tokens = []
    tokens.append(["M",'WC', 'PNL','ROI'])
    for correct in range(0,k+1):
        c = combos(correct,n)
        winnings = c * (2**n)
        pnl = '{0:>5}'.format(int((winnings*1.0) - cost))
        ROI = '{0:>3.1f}'.format((winnings*1.0) / cost)
        tokens.append([correct, c, pnl,ROI])

    print tokens_to_html_table(tokens)
    print '<br/>'
    print '</span>'

        



if __name__=='__main__':

    print '<html>'
    print '<head>'
    print '<title>Combinatorics Analysis For Even Money Bets</title>'

    f = open("table_style.js")
    for line in f:
        print line

    f = open("theme.css")
    for line in f:
        print line

    print '</head>'
    print '<body onload="doStuff()" style="width:700px" >'

    f = open("comments.txt")
    for line in f:
        print line
   
    for k in range(4,10):
   
        
        print "<div style='align:left;clear:left;margin-top:10px;width:720px;  margin-left:10px;border:3px solid;color:blue;background-color:#ddd;'>"
        print '<h1>Selections: {0}<br/></h1>'.format(k)
        for n in range( 1,k):
            show_behavior(k,n)
        print '<br clear="all" /> '
        print '</div>'


    print '</body>'

    print '</html>'

