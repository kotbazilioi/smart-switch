<!DOCTYPE html>
<html lang='en'>
<head>
<title>snmpv3_dummy.h\snmp_v3\snmp\examples\contrib - lwip.git - lwIP - A Lightweight TCPIP stack</title>
<meta name='generator' content='cgit v1.2.1'/>
<meta name='robots' content='index, nofollow'/>
<link rel='stylesheet' type='text/css' href='/cgit/cgit.css'/>
<link rel='shortcut icon' href='/gitweb/git-favicon.png'/>
<link rel='alternate' title='Atom feed' href='http://git.savannah.nongnu.org/cgit/lwip.git/atom/contrib/examples/snmp/snmp_v3/snmpv3_dummy.h?h=master' type='application/atom+xml'/>
<link rel='vcs-git' href='git://git.savannah.gnu.org/lwip.git' title='lwip.git Git repository'/>
<link rel='vcs-git' href='https://git.savannah.gnu.org/git/lwip.git' title='lwip.git Git repository'/>
<link rel='vcs-git' href='ssh://git.savannah.gnu.org/srv/git/lwip.git' title='lwip.git Git repository'/>
</head>
<body>
<div id='cgit'><table id='header'>
<tr>
<td class='logo' rowspan='2'><a href='/cgit/'><img src='/cgit/cgit.png' alt='cgit logo'/></a></td>
<td class='main'><a href='/cgit/'>index</a> : <a title='lwip.git' href='/cgit/lwip.git/'>lwip.git</a></td><td class='form'><form method='get'>
<select name='h' onchange='this.form.submit();'>
<option value='DEVEL-1_4_1'>DEVEL-1_4_1</option>
<option value='STABLE-2_0_0'>STABLE-2_0_0</option>
<option value='STABLE-2_1_x'>STABLE-2_1_x</option>
<option value='master' selected='selected'>master</option>
</select> <input type='submit' value='switch'/></form></td></tr>
<tr><td class='sub'>lwIP - A Lightweight TCPIP stack</td><td class='sub right'></td></tr></table>
<table class='tabs'><tr><td>
<a href='/cgit/lwip.git/'>summary</a><a href='/cgit/lwip.git/refs/'>refs</a><a href='/cgit/lwip.git/log/contrib/examples/snmp/snmp_v3/snmpv3_dummy.h'>log</a><a class='active' href='/cgit/lwip.git/tree/contrib/examples/snmp/snmp_v3/snmpv3_dummy.h'>tree</a><a href='/cgit/lwip.git/commit/contrib/examples/snmp/snmp_v3/snmpv3_dummy.h'>commit</a><a href='/cgit/lwip.git/diff/contrib/examples/snmp/snmp_v3/snmpv3_dummy.h'>diff</a></td><td class='form'><form class='right' method='get' action='/cgit/lwip.git/log/contrib/examples/snmp/snmp_v3/snmpv3_dummy.h'>
<select name='qt'>
<option value='grep'>log msg</option>
<option value='author'>author</option>
<option value='committer'>committer</option>
<option value='range'>range</option>
</select>
<input class='txt' type='search' size='10' name='q' value=''/>
<input type='submit' value='search'/>
</form>
</td></tr></table>
<div class='path'>path: <a href='/cgit/lwip.git/tree/'>root</a>/<a href='/cgit/lwip.git/tree/contrib'>contrib</a>/<a href='/cgit/lwip.git/tree/contrib/examples'>examples</a>/<a href='/cgit/lwip.git/tree/contrib/examples/snmp'>snmp</a>/<a href='/cgit/lwip.git/tree/contrib/examples/snmp/snmp_v3'>snmp_v3</a>/<a href='/cgit/lwip.git/tree/contrib/examples/snmp/snmp_v3/snmpv3_dummy.h'>snmpv3_dummy.h</a></div><div class='content'>blob: ba25b6d7d5863f350e7c2cd3da3879a0c033514f (<a href='/cgit/lwip.git/plain/contrib/examples/snmp/snmp_v3/snmpv3_dummy.h'>plain</a>)
<table summary='blob content' class='blob'>
<tr><td class='linenumbers'><pre><a id='n1' href='#n1'>1</a>
<a id='n2' href='#n2'>2</a>
<a id='n3' href='#n3'>3</a>
<a id='n4' href='#n4'>4</a>
<a id='n5' href='#n5'>5</a>
<a id='n6' href='#n6'>6</a>
<a id='n7' href='#n7'>7</a>
<a id='n8' href='#n8'>8</a>
<a id='n9' href='#n9'>9</a>
<a id='n10' href='#n10'>10</a>
<a id='n11' href='#n11'>11</a>
<a id='n12' href='#n12'>12</a>
<a id='n13' href='#n13'>13</a>
<a id='n14' href='#n14'>14</a>
<a id='n15' href='#n15'>15</a>
<a id='n16' href='#n16'>16</a>
<a id='n17' href='#n17'>17</a>
<a id='n18' href='#n18'>18</a>
<a id='n19' href='#n19'>19</a>
<a id='n20' href='#n20'>20</a>
<a id='n21' href='#n21'>21</a>
<a id='n22' href='#n22'>22</a>
<a id='n23' href='#n23'>23</a>
<a id='n24' href='#n24'>24</a>
<a id='n25' href='#n25'>25</a>
<a id='n26' href='#n26'>26</a>
<a id='n27' href='#n27'>27</a>
<a id='n28' href='#n28'>28</a>
<a id='n29' href='#n29'>29</a>
<a id='n30' href='#n30'>30</a>
<a id='n31' href='#n31'>31</a>
<a id='n32' href='#n32'>32</a>
<a id='n33' href='#n33'>33</a>
<a id='n34' href='#n34'>34</a>
<a id='n35' href='#n35'>35</a>
<a id='n36' href='#n36'>36</a>
<a id='n37' href='#n37'>37</a>
<a id='n38' href='#n38'>38</a>
<a id='n39' href='#n39'>39</a>
<a id='n40' href='#n40'>40</a>
<a id='n41' href='#n41'>41</a>
<a id='n42' href='#n42'>42</a>
<a id='n43' href='#n43'>43</a>
<a id='n44' href='#n44'>44</a>
<a id='n45' href='#n45'>45</a>
<a id='n46' href='#n46'>46</a>
<a id='n47' href='#n47'>47</a>
<a id='n48' href='#n48'>48</a>
<a id='n49' href='#n49'>49</a>
<a id='n50' href='#n50'>50</a>
<a id='n51' href='#n51'>51</a>
<a id='n52' href='#n52'>52</a>
<a id='n53' href='#n53'>53</a>
</pre></td>
<td class='lines'><pre><code>/**
 * @file
 * Dummy SNMPv3 functions.
 */

/*
 * Copyright (c) 2017 Dirk Ziegelmeier.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Dirk Ziegelmeier &lt;dziegel@gmx.de&gt;
 */

#ifndef LWIP_HDR_APPS_SNMP_V3_DUMMY_H
#define LWIP_HDR_APPS_SNMP_V3_DUMMY_H

#include "lwip/apps/snmp_opts.h"
#include "lwip/err.h"
#include "lwip/apps/snmpv3.h"

#if LWIP_SNMP &amp;&amp; LWIP_SNMP_V3

err_t snmpv3_set_user_auth_algo(const char *username, snmpv3_auth_algo_t algo);
err_t snmpv3_set_user_priv_algo(const char *username, snmpv3_priv_algo_t algo);
err_t snmpv3_set_user_auth_key(const char *username, const char *password);
err_t snmpv3_set_user_priv_key(const char *username, const char *password);

void snmpv3_dummy_init(void);

#endif /* LWIP_SNMP &amp;&amp; LWIP_SNMP_V3 */

#endif /* LWIP_HDR_APPS_SNMP_V3_DUMMY_H */
</code></pre></td></tr></table>
</div> <!-- class=content -->
<div class='footer'>generated by <a href='https://git.zx2c4.com/cgit/about/'>cgit v1.2.1</a> (<a href='https://git-scm.com/'>git 2.18.0</a>) at 2021-02-14 16:45:06 +0000</div>
</div> <!-- id=cgit -->
</body>
</html>
