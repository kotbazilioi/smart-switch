<!DOCTYPE html>
<html lang='en'>
<head>
<title>snmpv3_dummy.c\snmp_v3\snmp\examples\contrib - lwip.git - lwIP - A Lightweight TCPIP stack</title>
<meta name='generator' content='cgit v1.2.1'/>
<meta name='robots' content='index, nofollow'/>
<link rel='stylesheet' type='text/css' href='/cgit/cgit.css'/>
<link rel='shortcut icon' href='/gitweb/git-favicon.png'/>
<link rel='alternate' title='Atom feed' href='http://git.savannah.nongnu.org/cgit/lwip.git/atom/contrib/examples/snmp/snmp_v3/snmpv3_dummy.c?h=master' type='application/atom+xml'/>
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
<a href='/cgit/lwip.git/'>summary</a><a href='/cgit/lwip.git/refs/'>refs</a><a href='/cgit/lwip.git/log/contrib/examples/snmp/snmp_v3/snmpv3_dummy.c'>log</a><a class='active' href='/cgit/lwip.git/tree/contrib/examples/snmp/snmp_v3/snmpv3_dummy.c'>tree</a><a href='/cgit/lwip.git/commit/contrib/examples/snmp/snmp_v3/snmpv3_dummy.c'>commit</a><a href='/cgit/lwip.git/diff/contrib/examples/snmp/snmp_v3/snmpv3_dummy.c'>diff</a></td><td class='form'><form class='right' method='get' action='/cgit/lwip.git/log/contrib/examples/snmp/snmp_v3/snmpv3_dummy.c'>
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
<div class='path'>path: <a href='/cgit/lwip.git/tree/'>root</a>/<a href='/cgit/lwip.git/tree/contrib'>contrib</a>/<a href='/cgit/lwip.git/tree/contrib/examples'>examples</a>/<a href='/cgit/lwip.git/tree/contrib/examples/snmp'>snmp</a>/<a href='/cgit/lwip.git/tree/contrib/examples/snmp/snmp_v3'>snmp_v3</a>/<a href='/cgit/lwip.git/tree/contrib/examples/snmp/snmp_v3/snmpv3_dummy.c'>snmpv3_dummy.c</a></div><div class='content'>blob: a7c53ed73ac07ed9d23f9fe832b323540ea98ee4 (<a href='/cgit/lwip.git/plain/contrib/examples/snmp/snmp_v3/snmpv3_dummy.c'>plain</a>)
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
<a id='n54' href='#n54'>54</a>
<a id='n55' href='#n55'>55</a>
<a id='n56' href='#n56'>56</a>
<a id='n57' href='#n57'>57</a>
<a id='n58' href='#n58'>58</a>
<a id='n59' href='#n59'>59</a>
<a id='n60' href='#n60'>60</a>
<a id='n61' href='#n61'>61</a>
<a id='n62' href='#n62'>62</a>
<a id='n63' href='#n63'>63</a>
<a id='n64' href='#n64'>64</a>
<a id='n65' href='#n65'>65</a>
<a id='n66' href='#n66'>66</a>
<a id='n67' href='#n67'>67</a>
<a id='n68' href='#n68'>68</a>
<a id='n69' href='#n69'>69</a>
<a id='n70' href='#n70'>70</a>
<a id='n71' href='#n71'>71</a>
<a id='n72' href='#n72'>72</a>
<a id='n73' href='#n73'>73</a>
<a id='n74' href='#n74'>74</a>
<a id='n75' href='#n75'>75</a>
<a id='n76' href='#n76'>76</a>
<a id='n77' href='#n77'>77</a>
<a id='n78' href='#n78'>78</a>
<a id='n79' href='#n79'>79</a>
<a id='n80' href='#n80'>80</a>
<a id='n81' href='#n81'>81</a>
<a id='n82' href='#n82'>82</a>
<a id='n83' href='#n83'>83</a>
<a id='n84' href='#n84'>84</a>
<a id='n85' href='#n85'>85</a>
<a id='n86' href='#n86'>86</a>
<a id='n87' href='#n87'>87</a>
<a id='n88' href='#n88'>88</a>
<a id='n89' href='#n89'>89</a>
<a id='n90' href='#n90'>90</a>
<a id='n91' href='#n91'>91</a>
<a id='n92' href='#n92'>92</a>
<a id='n93' href='#n93'>93</a>
<a id='n94' href='#n94'>94</a>
<a id='n95' href='#n95'>95</a>
<a id='n96' href='#n96'>96</a>
<a id='n97' href='#n97'>97</a>
<a id='n98' href='#n98'>98</a>
<a id='n99' href='#n99'>99</a>
<a id='n100' href='#n100'>100</a>
<a id='n101' href='#n101'>101</a>
<a id='n102' href='#n102'>102</a>
<a id='n103' href='#n103'>103</a>
<a id='n104' href='#n104'>104</a>
<a id='n105' href='#n105'>105</a>
<a id='n106' href='#n106'>106</a>
<a id='n107' href='#n107'>107</a>
<a id='n108' href='#n108'>108</a>
<a id='n109' href='#n109'>109</a>
<a id='n110' href='#n110'>110</a>
<a id='n111' href='#n111'>111</a>
<a id='n112' href='#n112'>112</a>
<a id='n113' href='#n113'>113</a>
<a id='n114' href='#n114'>114</a>
<a id='n115' href='#n115'>115</a>
<a id='n116' href='#n116'>116</a>
<a id='n117' href='#n117'>117</a>
<a id='n118' href='#n118'>118</a>
<a id='n119' href='#n119'>119</a>
<a id='n120' href='#n120'>120</a>
<a id='n121' href='#n121'>121</a>
<a id='n122' href='#n122'>122</a>
<a id='n123' href='#n123'>123</a>
<a id='n124' href='#n124'>124</a>
<a id='n125' href='#n125'>125</a>
<a id='n126' href='#n126'>126</a>
<a id='n127' href='#n127'>127</a>
<a id='n128' href='#n128'>128</a>
<a id='n129' href='#n129'>129</a>
<a id='n130' href='#n130'>130</a>
<a id='n131' href='#n131'>131</a>
<a id='n132' href='#n132'>132</a>
<a id='n133' href='#n133'>133</a>
<a id='n134' href='#n134'>134</a>
<a id='n135' href='#n135'>135</a>
<a id='n136' href='#n136'>136</a>
<a id='n137' href='#n137'>137</a>
<a id='n138' href='#n138'>138</a>
<a id='n139' href='#n139'>139</a>
<a id='n140' href='#n140'>140</a>
<a id='n141' href='#n141'>141</a>
<a id='n142' href='#n142'>142</a>
<a id='n143' href='#n143'>143</a>
<a id='n144' href='#n144'>144</a>
<a id='n145' href='#n145'>145</a>
<a id='n146' href='#n146'>146</a>
<a id='n147' href='#n147'>147</a>
<a id='n148' href='#n148'>148</a>
<a id='n149' href='#n149'>149</a>
<a id='n150' href='#n150'>150</a>
<a id='n151' href='#n151'>151</a>
<a id='n152' href='#n152'>152</a>
<a id='n153' href='#n153'>153</a>
<a id='n154' href='#n154'>154</a>
<a id='n155' href='#n155'>155</a>
<a id='n156' href='#n156'>156</a>
<a id='n157' href='#n157'>157</a>
<a id='n158' href='#n158'>158</a>
<a id='n159' href='#n159'>159</a>
<a id='n160' href='#n160'>160</a>
<a id='n161' href='#n161'>161</a>
<a id='n162' href='#n162'>162</a>
<a id='n163' href='#n163'>163</a>
<a id='n164' href='#n164'>164</a>
<a id='n165' href='#n165'>165</a>
<a id='n166' href='#n166'>166</a>
<a id='n167' href='#n167'>167</a>
<a id='n168' href='#n168'>168</a>
<a id='n169' href='#n169'>169</a>
<a id='n170' href='#n170'>170</a>
<a id='n171' href='#n171'>171</a>
<a id='n172' href='#n172'>172</a>
<a id='n173' href='#n173'>173</a>
<a id='n174' href='#n174'>174</a>
<a id='n175' href='#n175'>175</a>
<a id='n176' href='#n176'>176</a>
<a id='n177' href='#n177'>177</a>
<a id='n178' href='#n178'>178</a>
<a id='n179' href='#n179'>179</a>
<a id='n180' href='#n180'>180</a>
<a id='n181' href='#n181'>181</a>
<a id='n182' href='#n182'>182</a>
<a id='n183' href='#n183'>183</a>
<a id='n184' href='#n184'>184</a>
<a id='n185' href='#n185'>185</a>
<a id='n186' href='#n186'>186</a>
<a id='n187' href='#n187'>187</a>
<a id='n188' href='#n188'>188</a>
<a id='n189' href='#n189'>189</a>
<a id='n190' href='#n190'>190</a>
<a id='n191' href='#n191'>191</a>
<a id='n192' href='#n192'>192</a>
<a id='n193' href='#n193'>193</a>
<a id='n194' href='#n194'>194</a>
<a id='n195' href='#n195'>195</a>
<a id='n196' href='#n196'>196</a>
<a id='n197' href='#n197'>197</a>
<a id='n198' href='#n198'>198</a>
<a id='n199' href='#n199'>199</a>
<a id='n200' href='#n200'>200</a>
<a id='n201' href='#n201'>201</a>
<a id='n202' href='#n202'>202</a>
<a id='n203' href='#n203'>203</a>
<a id='n204' href='#n204'>204</a>
<a id='n205' href='#n205'>205</a>
<a id='n206' href='#n206'>206</a>
<a id='n207' href='#n207'>207</a>
<a id='n208' href='#n208'>208</a>
<a id='n209' href='#n209'>209</a>
<a id='n210' href='#n210'>210</a>
<a id='n211' href='#n211'>211</a>
<a id='n212' href='#n212'>212</a>
<a id='n213' href='#n213'>213</a>
<a id='n214' href='#n214'>214</a>
<a id='n215' href='#n215'>215</a>
<a id='n216' href='#n216'>216</a>
<a id='n217' href='#n217'>217</a>
<a id='n218' href='#n218'>218</a>
<a id='n219' href='#n219'>219</a>
<a id='n220' href='#n220'>220</a>
<a id='n221' href='#n221'>221</a>
<a id='n222' href='#n222'>222</a>
<a id='n223' href='#n223'>223</a>
<a id='n224' href='#n224'>224</a>
<a id='n225' href='#n225'>225</a>
<a id='n226' href='#n226'>226</a>
<a id='n227' href='#n227'>227</a>
<a id='n228' href='#n228'>228</a>
<a id='n229' href='#n229'>229</a>
<a id='n230' href='#n230'>230</a>
<a id='n231' href='#n231'>231</a>
<a id='n232' href='#n232'>232</a>
<a id='n233' href='#n233'>233</a>
<a id='n234' href='#n234'>234</a>
<a id='n235' href='#n235'>235</a>
<a id='n236' href='#n236'>236</a>
<a id='n237' href='#n237'>237</a>
<a id='n238' href='#n238'>238</a>
<a id='n239' href='#n239'>239</a>
<a id='n240' href='#n240'>240</a>
<a id='n241' href='#n241'>241</a>
<a id='n242' href='#n242'>242</a>
<a id='n243' href='#n243'>243</a>
<a id='n244' href='#n244'>244</a>
<a id='n245' href='#n245'>245</a>
<a id='n246' href='#n246'>246</a>
<a id='n247' href='#n247'>247</a>
<a id='n248' href='#n248'>248</a>
<a id='n249' href='#n249'>249</a>
<a id='n250' href='#n250'>250</a>
<a id='n251' href='#n251'>251</a>
<a id='n252' href='#n252'>252</a>
<a id='n253' href='#n253'>253</a>
<a id='n254' href='#n254'>254</a>
<a id='n255' href='#n255'>255</a>
<a id='n256' href='#n256'>256</a>
<a id='n257' href='#n257'>257</a>
<a id='n258' href='#n258'>258</a>
<a id='n259' href='#n259'>259</a>
<a id='n260' href='#n260'>260</a>
<a id='n261' href='#n261'>261</a>
<a id='n262' href='#n262'>262</a>
<a id='n263' href='#n263'>263</a>
<a id='n264' href='#n264'>264</a>
<a id='n265' href='#n265'>265</a>
<a id='n266' href='#n266'>266</a>
<a id='n267' href='#n267'>267</a>
<a id='n268' href='#n268'>268</a>
<a id='n269' href='#n269'>269</a>
<a id='n270' href='#n270'>270</a>
<a id='n271' href='#n271'>271</a>
<a id='n272' href='#n272'>272</a>
<a id='n273' href='#n273'>273</a>
<a id='n274' href='#n274'>274</a>
<a id='n275' href='#n275'>275</a>
<a id='n276' href='#n276'>276</a>
<a id='n277' href='#n277'>277</a>
<a id='n278' href='#n278'>278</a>
<a id='n279' href='#n279'>279</a>
<a id='n280' href='#n280'>280</a>
<a id='n281' href='#n281'>281</a>
<a id='n282' href='#n282'>282</a>
<a id='n283' href='#n283'>283</a>
<a id='n284' href='#n284'>284</a>
<a id='n285' href='#n285'>285</a>
<a id='n286' href='#n286'>286</a>
<a id='n287' href='#n287'>287</a>
<a id='n288' href='#n288'>288</a>
<a id='n289' href='#n289'>289</a>
<a id='n290' href='#n290'>290</a>
<a id='n291' href='#n291'>291</a>
<a id='n292' href='#n292'>292</a>
<a id='n293' href='#n293'>293</a>
<a id='n294' href='#n294'>294</a>
<a id='n295' href='#n295'>295</a>
<a id='n296' href='#n296'>296</a>
<a id='n297' href='#n297'>297</a>
<a id='n298' href='#n298'>298</a>
<a id='n299' href='#n299'>299</a>
<a id='n300' href='#n300'>300</a>
<a id='n301' href='#n301'>301</a>
<a id='n302' href='#n302'>302</a>
<a id='n303' href='#n303'>303</a>
<a id='n304' href='#n304'>304</a>
<a id='n305' href='#n305'>305</a>
<a id='n306' href='#n306'>306</a>
<a id='n307' href='#n307'>307</a>
<a id='n308' href='#n308'>308</a>
<a id='n309' href='#n309'>309</a>
<a id='n310' href='#n310'>310</a>
<a id='n311' href='#n311'>311</a>
<a id='n312' href='#n312'>312</a>
<a id='n313' href='#n313'>313</a>
<a id='n314' href='#n314'>314</a>
<a id='n315' href='#n315'>315</a>
<a id='n316' href='#n316'>316</a>
<a id='n317' href='#n317'>317</a>
<a id='n318' href='#n318'>318</a>
<a id='n319' href='#n319'>319</a>
<a id='n320' href='#n320'>320</a>
<a id='n321' href='#n321'>321</a>
<a id='n322' href='#n322'>322</a>
<a id='n323' href='#n323'>323</a>
<a id='n324' href='#n324'>324</a>
<a id='n325' href='#n325'>325</a>
<a id='n326' href='#n326'>326</a>
<a id='n327' href='#n327'>327</a>
<a id='n328' href='#n328'>328</a>
<a id='n329' href='#n329'>329</a>
<a id='n330' href='#n330'>330</a>
<a id='n331' href='#n331'>331</a>
<a id='n332' href='#n332'>332</a>
<a id='n333' href='#n333'>333</a>
<a id='n334' href='#n334'>334</a>
<a id='n335' href='#n335'>335</a>
<a id='n336' href='#n336'>336</a>
<a id='n337' href='#n337'>337</a>
<a id='n338' href='#n338'>338</a>
<a id='n339' href='#n339'>339</a>
<a id='n340' href='#n340'>340</a>
<a id='n341' href='#n341'>341</a>
<a id='n342' href='#n342'>342</a>
<a id='n343' href='#n343'>343</a>
<a id='n344' href='#n344'>344</a>
<a id='n345' href='#n345'>345</a>
<a id='n346' href='#n346'>346</a>
<a id='n347' href='#n347'>347</a>
<a id='n348' href='#n348'>348</a>
<a id='n349' href='#n349'>349</a>
<a id='n350' href='#n350'>350</a>
<a id='n351' href='#n351'>351</a>
<a id='n352' href='#n352'>352</a>
<a id='n353' href='#n353'>353</a>
<a id='n354' href='#n354'>354</a>
<a id='n355' href='#n355'>355</a>
<a id='n356' href='#n356'>356</a>
<a id='n357' href='#n357'>357</a>
<a id='n358' href='#n358'>358</a>
<a id='n359' href='#n359'>359</a>
<a id='n360' href='#n360'>360</a>
<a id='n361' href='#n361'>361</a>
<a id='n362' href='#n362'>362</a>
<a id='n363' href='#n363'>363</a>
<a id='n364' href='#n364'>364</a>
<a id='n365' href='#n365'>365</a>
<a id='n366' href='#n366'>366</a>
<a id='n367' href='#n367'>367</a>
<a id='n368' href='#n368'>368</a>
<a id='n369' href='#n369'>369</a>
<a id='n370' href='#n370'>370</a>
<a id='n371' href='#n371'>371</a>
<a id='n372' href='#n372'>372</a>
<a id='n373' href='#n373'>373</a>
<a id='n374' href='#n374'>374</a>
<a id='n375' href='#n375'>375</a>
<a id='n376' href='#n376'>376</a>
<a id='n377' href='#n377'>377</a>
<a id='n378' href='#n378'>378</a>
<a id='n379' href='#n379'>379</a>
<a id='n380' href='#n380'>380</a>
<a id='n381' href='#n381'>381</a>
<a id='n382' href='#n382'>382</a>
<a id='n383' href='#n383'>383</a>
<a id='n384' href='#n384'>384</a>
<a id='n385' href='#n385'>385</a>
<a id='n386' href='#n386'>386</a>
<a id='n387' href='#n387'>387</a>
<a id='n388' href='#n388'>388</a>
<a id='n389' href='#n389'>389</a>
<a id='n390' href='#n390'>390</a>
<a id='n391' href='#n391'>391</a>
<a id='n392' href='#n392'>392</a>
<a id='n393' href='#n393'>393</a>
<a id='n394' href='#n394'>394</a>
<a id='n395' href='#n395'>395</a>
</pre></td>
<td class='lines'><pre><code>/**
 * @file
 * Dummy SNMPv3 functions.
 */

/*
 * Copyright (c) 2016 Elias Oenal.
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
 * Author: Elias Oenal &lt;lwip@eliasoenal.com&gt;
 *         Dirk Ziegelmeier &lt;dirk@ziegelmeier.net&gt;
 */

#include "lwip/apps/snmpv3.h"
#include "snmpv3_dummy.h"
#include &lt;string.h&gt;
#include "lwip/err.h"
#include "lwip/def.h"
#include "lwip/timeouts.h"

#if LWIP_SNMP &amp;&amp; LWIP_SNMP_V3

struct user_table_entry {
  char               username[32];
  snmpv3_auth_algo_t auth_algo;
  u8_t               auth_key[20];
  snmpv3_priv_algo_t priv_algo;
  u8_t               priv_key[20];
};

static struct user_table_entry user_table[] = {
  { "lwip", SNMP_V3_AUTH_ALGO_INVAL, "" , SNMP_V3_PRIV_ALGO_INVAL, "" },
  { "piwl", SNMP_V3_AUTH_ALGO_INVAL, "" , SNMP_V3_PRIV_ALGO_INVAL, "" },
  { "test", SNMP_V3_AUTH_ALGO_INVAL, "" , SNMP_V3_PRIV_ALGO_INVAL, "" }
};

static char snmpv3_engineid[32];
static u8_t snmpv3_engineid_len;

static u32_t enginetime = 0;

/* In this implementation engineboots is volatile. In a real world application this value should be stored in non-volatile memory.*/
static u32_t engineboots = 0;

/**
 * @brief   Get the user table entry for the given username.
 *
 * @param[in] username  pointer to the username
 *
 * @return              pointer to the user table entry or NULL if not found.
 */
static struct user_table_entry*
get_user(const char *username)
{
  size_t i;

  for (i = 0; i &lt; LWIP_ARRAYSIZE(user_table); i++) {
    if (strnlen(username, 32) != strnlen(user_table[i].username, 32)) {
      continue;
    }

    if (memcmp(username, user_table[i].username, strnlen(username, 32)) == 0) {
      return &amp;user_table[i];
    }
  }

  return NULL;
}

u8_t
snmpv3_get_amount_of_users(void)
{
  return LWIP_ARRAYSIZE(user_table);
}

/**
 * @brief Get the username of a user number (index)
 * @param username is a pointer to a string.
 * @param index is the user index.
 * @return ERR_OK if user is found, ERR_VAL is user is not found.
 */
err_t
snmpv3_get_username(char *username, u8_t index)
{
  if (index &lt; LWIP_ARRAYSIZE(user_table)) {
    MEMCPY(username, user_table[index].username, sizeof(user_table[0].username));
    return ERR_OK;
  }

  return ERR_VAL;
}

/**
 * Timer callback function that increments enginetime and reschedules itself.
 *
 * @param arg unused argument
 */
static void
snmpv3_enginetime_timer(void *arg)
{
  LWIP_UNUSED_ARG(arg);
  
  enginetime++;

  /* This handles the engine time reset */
  snmpv3_get_engine_time_internal();

  /* restart timer */
  sys_timeout(1000, snmpv3_enginetime_timer, NULL);
}

err_t
snmpv3_set_user_auth_algo(const char *username, snmpv3_auth_algo_t algo)
{
  struct user_table_entry *p = get_user(username);

  if (p) {
    switch (algo) {
    case SNMP_V3_AUTH_ALGO_INVAL:
      if (p-&gt;priv_algo != SNMP_V3_PRIV_ALGO_INVAL) {
        /* Privacy MUST be disabled before configuring authentication */
        break;
      } else {
        p-&gt;auth_algo = algo;
        return ERR_OK;
      }
#if LWIP_SNMP_V3_CRYPTO
    case SNMP_V3_AUTH_ALGO_MD5:
    case SNMP_V3_AUTH_ALGO_SHA:
#endif
      p-&gt;auth_algo = algo;
      return ERR_OK;
    default:
      break;
    }
  }

  return ERR_VAL;
}

err_t
snmpv3_set_user_priv_algo(const char *username, snmpv3_priv_algo_t algo)
{
  struct user_table_entry *p = get_user(username);

  if (p) {
    switch (algo) {
#if LWIP_SNMP_V3_CRYPTO
    case SNMP_V3_PRIV_ALGO_AES:
    case SNMP_V3_PRIV_ALGO_DES:
      if (p-&gt;auth_algo == SNMP_V3_AUTH_ALGO_INVAL) {
        /* Authentication MUST be enabled before configuring privacy */
        break;
      } else {
        p-&gt;priv_algo = algo;
        return ERR_OK;
      }
#endif
    case SNMP_V3_PRIV_ALGO_INVAL:
      p-&gt;priv_algo = algo;
      return ERR_OK;
    default:
      break;
    }
  }

  return ERR_VAL;
}

err_t
snmpv3_set_user_auth_key(const char *username, const char *password)
{
  struct user_table_entry *p = get_user(username);
  const char *engineid;
  u8_t engineid_len;

  if (p) {
    /* password should be at least 8 characters long */
    if (strlen(password) &gt;= 8) {
      memset(p-&gt;auth_key, 0, sizeof(p-&gt;auth_key));
      snmpv3_get_engine_id(&amp;engineid, &amp;engineid_len);
      switch (p-&gt;auth_algo) {
      case SNMP_V3_AUTH_ALGO_INVAL:
        return ERR_OK;
#if LWIP_SNMP_V3_CRYPTO
      case SNMP_V3_AUTH_ALGO_MD5:
        snmpv3_password_to_key_md5((const u8_t*)password, strlen(password), (const u8_t*)engineid, engineid_len, p-&gt;auth_key);
        return ERR_OK;
      case SNMP_V3_AUTH_ALGO_SHA:
        snmpv3_password_to_key_sha((const u8_t*)password, strlen(password), (const u8_t*)engineid, engineid_len, p-&gt;auth_key);
        return ERR_OK;
#endif
      default:
        return ERR_VAL;
      }
    }
  }

  return ERR_VAL;
}

err_t
snmpv3_set_user_priv_key(const char *username, const char *password)
{
  struct user_table_entry *p = get_user(username);
  const char *engineid;
  u8_t engineid_len;

  if (p) {
    /* password should be at least 8 characters long */
    if (strlen(password) &gt;= 8) {
      memset(p-&gt;priv_key, 0, sizeof(p-&gt;priv_key));
      snmpv3_get_engine_id(&amp;engineid, &amp;engineid_len);
      switch (p-&gt;auth_algo) {
      case SNMP_V3_AUTH_ALGO_INVAL:
        return ERR_OK;
#if LWIP_SNMP_V3_CRYPTO
      case SNMP_V3_AUTH_ALGO_MD5:
        snmpv3_password_to_key_md5((const u8_t*)password, strlen(password), (const u8_t*)engineid, engineid_len, p-&gt;priv_key);
        return ERR_OK;
      case SNMP_V3_AUTH_ALGO_SHA:
        snmpv3_password_to_key_sha((const u8_t*)password, strlen(password), (const u8_t*)engineid, engineid_len, p-&gt;priv_key);
        return ERR_OK;
#endif
      default:
        return ERR_VAL;
      }
    }
  }

  return ERR_VAL;
}

/**
 * @brief   Get the storage type of the given username.
 *
 * @param[in] username  pointer to the username
 * @param[out] type     the storage type
 *
 * @return              ERR_OK if the user was found, ERR_VAL if not.
 */
err_t
snmpv3_get_user_storagetype(const char *username, snmpv3_user_storagetype_t *type)
{
  if (get_user(username) != NULL) {
    /* Found user in user table
     * In this dummy implementation, storage is permanent because no user can be deleted.
     * All changes to users are lost after a reboot.*/
    *type = SNMP_V3_USER_STORAGETYPE_PERMANENT;
    return ERR_OK;
  }

  return ERR_VAL;
}

/**
 *  @param username is a pointer to a string.
 * @param auth_algo is a pointer to u8_t. The implementation has to set this if user was found.
 * @param auth_key is a pointer to a pointer to a string. Implementation has to set this if user was found.
 * @param priv_algo is a pointer to u8_t. The implementation has to set this if user was found.
 * @param priv_key is a pointer to a pointer to a string. Implementation has to set this if user was found.
 */
err_t
snmpv3_get_user(const char* username, snmpv3_auth_algo_t *auth_algo, u8_t *auth_key, snmpv3_priv_algo_t *priv_algo, u8_t *priv_key)
{
  const struct user_table_entry *p;
  
  /* The msgUserName specifies the user (principal) on whose behalf the
     message is being exchanged. Note that a zero-length userName will
     not match any user, but it can be used for snmpEngineID discovery. */
  if(strlen(username) == 0) {
    return ERR_OK;
  }
  
  p = get_user(username);

  if (!p) {
    return ERR_VAL;
  }
  
  if (auth_algo != NULL) {
    *auth_algo = p-&gt;auth_algo;
  }
  if(auth_key != NULL) {
    MEMCPY(auth_key, p-&gt;auth_key, sizeof(p-&gt;auth_key));
  }
  if (priv_algo != NULL) {
    *priv_algo = p-&gt;priv_algo;
  }
  if(priv_key != NULL) {
    MEMCPY(priv_key, p-&gt;priv_key, sizeof(p-&gt;priv_key));
  }
  return ERR_OK;
}

/**
 * Get engine ID from persistence
 */
void
snmpv3_get_engine_id(const char **id, u8_t *len)
{
  *id = snmpv3_engineid;
  *len = snmpv3_engineid_len;
}

/**
 * Store engine ID in persistence
 */
err_t
snmpv3_set_engine_id(const char *id, u8_t len)
{
  MEMCPY(snmpv3_engineid, id, len);
  snmpv3_engineid_len = len;
  return ERR_OK;
}

/**
 * Get engine boots from persistence. Must be increased on each boot.
 */
u32_t
snmpv3_get_engine_boots(void)
{
  return engineboots;
}

/**
 * Store engine boots in persistence
 */
void 
snmpv3_set_engine_boots(u32_t boots)
{
  engineboots = boots;
}

/**
 * RFC3414 2.2.2.
 * Once the timer reaches 2147483647 it gets reset to zero and the
 * engine boot ups get incremented.
 */
u32_t
snmpv3_get_engine_time(void)
{
  return enginetime;
}

/**
 * Reset current engine time to 0
 */
void
snmpv3_reset_engine_time(void)
{
  enginetime = 0;
}

/**
 * Initialize dummy SNMPv3 implementation
 */
void
snmpv3_dummy_init(void)
{
  snmpv3_set_engine_id("FOO", 3);

  snmpv3_set_user_auth_algo("lwip", SNMP_V3_AUTH_ALGO_SHA);
  snmpv3_set_user_auth_key("lwip", "maplesyrup");

  snmpv3_set_user_priv_algo("lwip", SNMP_V3_PRIV_ALGO_DES);
  snmpv3_set_user_priv_key("lwip", "maplesyrup");

  /* Start the engine time timer */
  snmpv3_enginetime_timer(NULL);
}

#endif /* LWIP_SNMP &amp;&amp; LWIP_SNMP_V3 */
</code></pre></td></tr></table>
</div> <!-- class=content -->
<div class='footer'>generated by <a href='https://git.zx2c4.com/cgit/about/'>cgit v1.2.1</a> (<a href='https://git-scm.com/'>git 2.18.0</a>) at 2021-02-14 16:44:58 +0000</div>
</div> <!-- id=cgit -->
</body>
</html>
