
const float HighPassFirData[512] = {
    -46.05042552074078090E-6,
     194.0226881561529810E-6,
     156.7801964602884080E-6,
    -62.17501019692788590E-6,
     121.5215772034647730E-6,
     261.2162033676910370E-6,
    -153.3558953514087190E-9,
     21.95196400304525500E-6,
     302.5314475263634220E-6,
     131.1750606123817990E-6,
    -47.24606410811172450E-6,
     256.7107832367616420E-6,
     284.8499851389252060E-6,
    -33.72876508769219580E-6,
     142.8199948829454230E-6,
     394.4278288796967900E-6,
     80.63926913692085920E-6,
     20.08996571563615420E-6,
     406.2579839361620770E-6,
     264.3285689835819310E-6,
    -37.11285378667973590E-6,
     310.0804227184177080E-6,
     444.8330607295164900E-6,
     23.62670199782009210E-6,
     151.2324901175019530E-6,
     540.5180042979545760E-6,
     201.6684225640167650E-6,
     15.19233859876130130E-6,
     502.0709577828761210E-6,
     435.6320467599678640E-6,
    -11.35149676355496600E-6,
     342.9526442990259620E-6,
     626.5483096112440080E-6,
     114.3504556671516920E-6,
     141.2306672989206220E-6,
     683.9411220524448250E-6,
     362.6370599320562750E-6,
     7.912368745955669520E-6,
     573.7813102690613500E-6,
     636.8069786489858190E-6,
     33.86448833366641510E-6,
     343.5058716067798060E-6,
     813.9842946456897150E-6,
     240.1848600133690180E-6,
     108.1068933764337320E-6,
     804.8022812403212360E-6,
     558.2383283929314080E-6,
    -430.7364161348385210E-9,
     603.3520073124318510E-6,
     853.3981015342831140E-6,
     101.2494105049605080E-6,
     300.3988556859613370E-6,
     985.5942440049782360E-6,
     399.1894217432518990E-6,
     48.62471141883801810E-6,
     879.9354437252235360E-6,
     777.4113989824234070E-6,
    -7.935853489888787140E-6,
     572.3538129733885850E-6,
     0.001064749227768530,
     191.7192769442728210E-6,
     204.0258458824567070E-6,
     0.001115186189656357,
     584.9301399369518320E-6,
    -38.39354193304703960E-6,
     884.4876489181644960E-6,
     0.001003003981449317,
    -12.31596798071757880E-6,
     463.7016224472056360E-6,
     0.001244561392033929,
     303.7196793521905530E-6,
     47.79939690214921200E-6,
     0.001173404380429015,
     785.9252475878176940E-6,
    -151.6067585035136460E-6,
     793.9258463728551760E-6,
     0.001211929753063230,
    -11.20093148014502840E-6,
     263.5442675099256460E-6,
     0.001362057949002770,
     432.6351381712388500E-6,
    -170.6542578114920160E-6,
     0.001129739848667960,
     985.4336730707069590E-6,
    -286.8783110525714620E-6,
     586.3346532851672920E-6,
     0.001375868566604169,
    -2.528266287453826870E-6,
    -36.84372133306411000E-6,
     0.001383723605029484,
     570.3466290885708080E-6,
    -448.5008880898987510E-6,
     954.9509145590565140E-6,
     0.001161646180903278,
    -437.2219080072298420E-6,
     244.8351817959097330E-6,
     0.001462506758440839,
     15.00870824964375850E-6,
    -439.3295450892048850E-6,
     0.001275523983673587,
     704.9946268208663010E-6,
    -777.0041197592884150E-6,
     623.7246374477208520E-6,
     0.001288301819806543,
    -592.9873972531863730E-6,
    -240.0676754679395510E-6,
     0.001437257755730625,
     41.49492188193355700E-6,
    -937.5017966104594510E-6,
     0.001005454829515665,
     820.9861286780505910E-6,
    -0.001141202808859811,
     117.3737523066661200E-6,
     0.001335705579502841,
    -742.2777035088774940E-6,
    -868.4049236756624170E-6,
     0.001265343530867750,
     75.35287885772135040E-6,
    -0.001515687519323588,
     546.2208317616863270E-6,
     899.2552672712635060E-6,
    -0.001519990713583616,
    -573.6542483796788470E-6,
     0.001272068731464821,
    -871.5596316131095590E-6,
    -0.001629104706303308,
     914.0589010432622670E-6,
     112.9670077178890750E-6,
    -0.002148482711919008,
    -122.1921722684058120E-6,
     917.7475709466257290E-6,
    -0.001886576246523048,
    -0.001447674239405625,
     0.001065031906956082,
    -966.4020177077326250E-6,
    -0.002498921945932852,
     354.9850539654211160E-6,
     148.4005148015152710E-6,
    -0.002800799756984747,
    -0.001010308594880989,
     852.0455138774747180E-6,
    -0.002209233533588224,
    -0.002490253959612372,
     683.1601036761187520E-6,
    -0.001012244920404542,
    -0.003441984044078349,
    -434.1363347311527720E-6,
     173.2083593424758020E-6,
    -0.003428333500117393,
    -0.002117581094173629,
     675.9813937576582250E-6,
    -0.002452170123793975,
    -0.003673424372691273,
     97.10848210001364580E-6,
    -995.0706197750564570E-6,
    -0.004409880113573126,
    -0.001468222613915036,
     176.3093524715812070E-6,
    -0.003978201013209901,
    -0.003431695960429877,
     361.9761205484213060E-6,
    -0.002576214224344014,
    -0.004955640304521090,
    -719.9796162534848920E-6,
    -901.8021918103786450E-6,
    -0.005342035724671356,
    -0.002754824311282794,
     143.8110520472825440E-6,
    -0.004389276948853742,
    -0.004929033829226983,
    -119.3430242117483430E-6,
    -0.002538819676812428,
    -0.006282374616453406,
    -0.001793535812898627,
    -720.1744849941608210E-6,
    -0.006165800527920157,
    -0.004295940153077667,
     58.55282042319497290E-6,
    -0.004591318321702498,
    -0.006576612663990787,
    -801.0648263893258440E-6,
    -0.002292480704287266,
    -0.007586986008157385,
    -0.003151338650938563,
    -437.6428723644029330E-6,
    -0.006795059292517821,
    -0.006092903821398674,
    -101.1365421332974540E-6,
    -0.004501054291026662,
    -0.008336059195521571,
    -0.001725131186665005,
    -0.001779711633641141,
    -0.008791028319622581,
    -0.004832601681127131,
    -38.44218958180429980E-6,
    -0.007124757191805064,
    -0.008157007762391493,
    -364.6191848682416320E-6,
    -0.004011137077256822,
    -0.010170844273001269,
    -0.002954589887274600,
    -920.2808611483681030E-6,
    -0.009802046353118250,
    -0.006908716530518159,
     503.3653928386612510E-6,
    -0.007014900708210004,
    -0.010532683845069085,
    -777.0360426497938990E-6,
    -0.002961310719477792,
    -0.012060203804350272,
    -0.004603502194379716,
     421.0760162848844170E-6,
    -0.010503622373814105,
    -0.009533710035986936,
     0.001241102268569429,
    -0.006245367769849355,
    -0.013353847806022294,
    -0.001421301073134582,
    -0.001058944048237163,
    -0.014030949068949790,
    -0.006918553492171163,
     0.002525231029834681,
    -0.010720517072135508,
    -0.013086799820650794,
     0.002305041686424210,
    -0.004374630577200377,
    -0.017010098415674996,
    -0.002488046902927075,
     0.002378865870540136,
    -0.016252829590825212,
    -0.010552998357715846,
     0.006132567381821661,
    -0.010084339417367614,
    -0.018691303442795092,
     0.004088721068031485,
    -175.5237996663526020E-6,
    -0.022814727487077722,
    -0.004574462415259267,
     0.009572791595998689,
    -0.019465826831822727,
    -0.017899243565322708,
     0.014055566220243732,
    -0.007285142803097491,
    -0.031213939833566282,
     0.008402967665759650,
     0.012167328785591659,
    -0.037869041696484265,
    -0.011095322445271640,
     0.034614078118365255,
    -0.029314620192815583,
    -0.048947376399560441,
     0.054282984714981136,
     0.013561299423094390,
    -0.134182538301163029,
     0.065750738329173872,
     0.503345002900574379,
     0.503345002900574379,
     0.065750738329173872,
    -0.134182538301163029,
     0.013561299423094390,
     0.054282984714981136,
    -0.048947376399560441,
    -0.029314620192815583,
     0.034614078118365255,
    -0.011095322445271640,
    -0.037869041696484265,
     0.012167328785591659,
     0.008402967665759650,
    -0.031213939833566282,
    -0.007285142803097491,
     0.014055566220243732,
    -0.017899243565322708,
    -0.019465826831822727,
     0.009572791595998689,
    -0.004574462415259267,
    -0.022814727487077722,
    -175.5237996663526020E-6,
     0.004088721068031485,
    -0.018691303442795092,
    -0.010084339417367614,
     0.006132567381821661,
    -0.010552998357715846,
    -0.016252829590825212,
     0.002378865870540136,
    -0.002488046902927075,
    -0.017010098415674996,
    -0.004374630577200377,
     0.002305041686424210,
    -0.013086799820650794,
    -0.010720517072135508,
     0.002525231029834681,
    -0.006918553492171163,
    -0.014030949068949790,
    -0.001058944048237163,
    -0.001421301073134582,
    -0.013353847806022294,
    -0.006245367769849355,
     0.001241102268569429,
    -0.009533710035986936,
    -0.010503622373814105,
     421.0760162848844170E-6,
    -0.004603502194379716,
    -0.012060203804350272,
    -0.002961310719477792,
    -777.0360426497938990E-6,
    -0.010532683845069085,
    -0.007014900708210004,
     503.3653928386612510E-6,
    -0.006908716530518159,
    -0.009802046353118250,
    -920.2808611483681030E-6,
    -0.002954589887274600,
    -0.010170844273001269,
    -0.004011137077256822,
    -364.6191848682416320E-6,
    -0.008157007762391493,
    -0.007124757191805064,
    -38.44218958180429980E-6,
    -0.004832601681127131,
    -0.008791028319622581,
    -0.001779711633641141,
    -0.001725131186665005,
    -0.008336059195521571,
    -0.004501054291026662,
    -101.1365421332974540E-6,
    -0.006092903821398674,
    -0.006795059292517821,
    -437.6428723644029330E-6,
    -0.003151338650938563,
    -0.007586986008157385,
    -0.002292480704287266,
    -801.0648263893258440E-6,
    -0.006576612663990787,
    -0.004591318321702498,
     58.55282042319497290E-6,
    -0.004295940153077667,
    -0.006165800527920157,
    -720.1744849941608210E-6,
    -0.001793535812898627,
    -0.006282374616453406,
    -0.002538819676812428,
    -119.3430242117483430E-6,
    -0.004929033829226983,
    -0.004389276948853742,
     143.8110520472825440E-6,
    -0.002754824311282794,
    -0.005342035724671356,
    -901.8021918103786450E-6,
    -719.9796162534848920E-6,
    -0.004955640304521090,
    -0.002576214224344014,
     361.9761205484213060E-6,
    -0.003431695960429877,
    -0.003978201013209901,
     176.3093524715812070E-6,
    -0.001468222613915036,
    -0.004409880113573126,
    -995.0706197750564570E-6,
     97.10848210001364580E-6,
    -0.003673424372691273,
    -0.002452170123793975,
     675.9813937576582250E-6,
    -0.002117581094173629,
    -0.003428333500117393,
     173.2083593424758020E-6,
    -434.1363347311527720E-6,
    -0.003441984044078349,
    -0.001012244920404542,
     683.1601036761187520E-6,
    -0.002490253959612372,
    -0.002209233533588224,
     852.0455138774747180E-6,
    -0.001010308594880989,
    -0.002800799756984747,
     148.4005148015152710E-6,
     354.9850539654211160E-6,
    -0.002498921945932852,
    -966.4020177077326250E-6,
     0.001065031906956082,
    -0.001447674239405625,
    -0.001886576246523048,
     917.7475709466257290E-6,
    -122.1921722684058120E-6,
    -0.002148482711919008,
     112.9670077178890750E-6,
     914.0589010432622670E-6,
    -0.001629104706303308,
    -871.5596316131095590E-6,
     0.001272068731464821,
    -573.6542483796788470E-6,
    -0.001519990713583616,
     899.2552672712635060E-6,
     546.2208317616863270E-6,
    -0.001515687519323588,
     75.35287885772135040E-6,
     0.001265343530867750,
    -868.4049236756624170E-6,
    -742.2777035088774940E-6,
     0.001335705579502841,
     117.3737523066661200E-6,
    -0.001141202808859811,
     820.9861286780505910E-6,
     0.001005454829515665,
    -937.5017966104594510E-6,
     41.49492188193355700E-6,
     0.001437257755730625,
    -240.0676754679395510E-6,
    -592.9873972531863730E-6,
     0.001288301819806543,
     623.7246374477208520E-6,
    -777.0041197592884150E-6,
     704.9946268208663010E-6,
     0.001275523983673587,
    -439.3295450892048850E-6,
     15.00870824964375850E-6,
     0.001462506758440839,
     244.8351817959097330E-6,
    -437.2219080072298420E-6,
     0.001161646180903278,
     954.9509145590565140E-6,
    -448.5008880898987510E-6,
     570.3466290885708080E-6,
     0.001383723605029484,
    -36.84372133306411000E-6,
    -2.528266287453826870E-6,
     0.001375868566604169,
     586.3346532851672920E-6,
    -286.8783110525714620E-6,
     985.4336730707069590E-6,
     0.001129739848667960,
    -170.6542578114920160E-6,
     432.6351381712388500E-6,
     0.001362057949002770,
     263.5442675099256460E-6,
    -11.20093148014502840E-6,
     0.001211929753063230,
     793.9258463728551760E-6,
    -151.6067585035136460E-6,
     785.9252475878176940E-6,
     0.001173404380429015,
     47.79939690214921200E-6,
     303.7196793521905530E-6,
     0.001244561392033929,
     463.7016224472056360E-6,
    -12.31596798071757880E-6,
     0.001003003981449317,
     884.4876489181644960E-6,
    -38.39354193304703960E-6,
     584.9301399369518320E-6,
     0.001115186189656357,
     204.0258458824567070E-6,
     191.7192769442728210E-6,
     0.001064749227768530,
     572.3538129733885850E-6,
    -7.935853489888787140E-6,
     777.4113989824234070E-6,
     879.9354437252235360E-6,
     48.62471141883801810E-6,
     399.1894217432518990E-6,
     985.5942440049782360E-6,
     300.3988556859613370E-6,
     101.2494105049605080E-6,
     853.3981015342831140E-6,
     603.3520073124318510E-6,
    -430.7364161348385210E-9,
     558.2383283929314080E-6,
     804.8022812403212360E-6,
     108.1068933764337320E-6,
     240.1848600133690180E-6,
     813.9842946456897150E-6,
     343.5058716067798060E-6,
     33.86448833366641510E-6,
     636.8069786489858190E-6,
     573.7813102690613500E-6,
     7.912368745955669520E-6,
     362.6370599320562750E-6,
     683.9411220524448250E-6,
     141.2306672989206220E-6,
     114.3504556671516920E-6,
     626.5483096112440080E-6,
     342.9526442990259620E-6,
    -11.35149676355496600E-6,
     435.6320467599678640E-6,
     502.0709577828761210E-6,
     15.19233859876130130E-6,
     201.6684225640167650E-6,
     540.5180042979545760E-6,
     151.2324901175019530E-6,
     23.62670199782009210E-6,
     444.8330607295164900E-6,
     310.0804227184177080E-6,
    -37.11285378667973590E-6,
     264.3285689835819310E-6,
     406.2579839361620770E-6,
     20.08996571563615420E-6,
     80.63926913692085920E-6,
     394.4278288796967900E-6,
     142.8199948829454230E-6,
    -33.72876508769219580E-6,
     284.8499851389252060E-6,
     256.7107832367616420E-6,
    -47.24606410811172450E-6,
     131.1750606123817990E-6,
     302.5314475263634220E-6,
     21.95196400304525500E-6,
    -153.3558953514087190E-9,
     261.2162033676910370E-6,
     121.5215772034647730E-6,
    -62.17501019692788590E-6,
     156.7801964602884080E-6,
     194.0226881561529810E-6,
    -46.05042552074078090E-6
 };

