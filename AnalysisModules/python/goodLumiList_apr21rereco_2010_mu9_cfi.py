import FWCore.ParameterSet.Config as cms

# This configuration was derived
# /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions10/7TeV/Reprocessing/Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_CMSSWConfig.txt
# for use with the 4_2 reconstruction of the 2010 dataset.
# Only lumi sections for which HLT_Mu9 was UNPRESCALED are 
# enabled in this configuration.  For the 2010 high luminosity 
# configuration, please see 
# goodLumiList_apr21rereco_2010_mu15_cfi.py

lumisToProcess = cms.untracked.VLuminosityBlockRange(
	'136035:1-136035:53',
	'136035:55-136035:207',
	'136035:209-136035:259',
	'136066:181-136066:297',
	'136066:299-136066:319',
	'136066:321-136066:321',
	'136066:323-136066:336',
	'136066:338-136066:348',
	'136066:350-136066:366',
	'136066:368-136066:529',
	'136066:532-136066:544',
	'136066:546-136066:595',
	'136066:597-136066:719',
	'136066:721-136066:1004',
	'136066:1006-136066:1184',
	'136080:249-136080:250',
	'136080:252-136080:255',
	'136082:1-136082:173',
	'136082:175-136082:422',
	'136082:477-136082:504',
	'136082:506-136082:506',
	'136087:250-136087:265',
	'136087:267-136087:277',
	'136087:279-136087:310',
	'136087:312-136087:314',
	'136088:1-136088:3',
	'136088:6-136088:17',
	'136088:19-136088:39',
	'136088:41-136088:47',
	'136088:50-136088:50',
	'136088:52-136088:52',
	'136088:54-136088:63',
	'136088:65-136088:65',
	'136088:67-136088:89',
	'136088:91-136088:113',
	'136088:115-136088:131',
	'136088:133-136088:137',
	'136088:139-136088:184',
	'136088:187-136088:191',
	'136088:193-136088:194',
	'136088:196-136088:206',
	'136088:208-136088:208',
	'136088:210-136088:262',
	'136097:1-136097:1',
	'136097:3-136097:3',
	'136097:5-136097:5',
	'136097:7-136097:7',
	'136097:9-136097:9',
	'136097:11-136097:12',
	'136097:14-136097:78',
	'136098:1-136098:10',
	'136100:1-136100:94',
	'136119:1-136119:35',
	'137027:98-137027:146',
	'137027:149-137027:162',
	'137027:165-137027:187',
	'137028:1-137028:107',
	'137028:111-137028:157',
	'137028:163-137028:169',
	'137028:171-137028:180',
	'137028:184-137028:200',
	'137028:203-137028:209',
	'137028:211-137028:232',
	'137028:234-137028:274',
	'137028:276-137028:338',
	'137028:341-137028:475',
	'137028:478-137028:484',
	'138564:1-138564:4',
	'138571:1-138571:13',
	'138572:1-138572:213',
	'138737:1-138737:66',
	'138738:1-138738:10',
	'138739:1-138739:13',
	'138742:1-138742:20',
	'138742:22-138742:38',
	'138744:1-138744:24',
	'138745:1-138745:10',
	'138746:1-138746:129',
	'138747:1-138747:71',
	'138747:73-138747:131',
	'138750:1-138750:46',
	'138750:49-138750:208',
	'138750:210-138750:623',
	'138750:626-138750:709',
	'138751:1-138751:110',
	'138751:112-138751:146',
	'138919:62-138919:150',
	'138920:1-138920:55',
	'138921:1-138921:165',
	'138921:168-138921:181',
	'138921:183-138921:191',
	'138923:1-138923:2',
	'138924:1-138924:62',
	'138937:1-138937:26',
	'138939:1-138939:26',
	'139020:227-139020:316',
	'139020:319-139020:617',
	'139096:193-139096:210',
	'139096:212-139096:233',
	'139096:235-139096:236',
	'139096:239-139096:239',
	'139096:242-139096:267',
	'139098:1-139098:92',
	'139098:94-139098:121',
	'139098:123-139098:160',
	'139098:162-139098:179')
lumisToProcess.extend([
	'139100:1-139100:13',
	'139100:15-139100:15',
	'139100:17-139100:19',
	'139100:21-139100:102',
	'139100:104-139100:104',
	'139100:106-139100:205',
	'139100:207-139100:307',
	'139102:1-139102:12',
	'139102:14-139102:55',
	'139103:7-139103:154',
	'139103:157-139103:414',
	'139103:416-139103:416',
	'139103:418-139103:449',
	'139195:9-139195:14',
	'139195:16-139195:29',
	'139195:31-139195:81',
	'139195:83-139195:92',
	'139239:164-139239:214',
	'139239:217-139239:217',
	'139239:219-139239:222',
	'139239:224-139239:225',
	'139239:227-139239:270',
	'139347:214-139347:357',
	'139347:359-139347:516',
	'139356:175-139356:186',
	'139356:188-139356:192',
	'139360:1-139360:30',
	'139364:1-139364:48',
	'139364:51-139364:72',
	'139365:1-139365:9',
	'139365:12-139365:14',
	'139365:16-139365:82',
	'139365:84-139365:84',
	'139365:86-139365:112',
	'139365:114-139365:166',
	'139365:168-139365:179',
	'139365:181-139365:250',
	'139368:1-139368:34',
	'139368:36-139368:100',
	'139368:102-139368:150',
	'139368:152-139368:166',
	'139370:1-139370:175',
	'139370:177-139370:254',
	'139370:257-139370:445',
	'139370:447-139370:468',
	'139370:472-139370:504',
	'139370:506-139370:562',
	'139370:564-139370:581',
	'139370:583-139370:619',
	'139370:622-139370:645',
	'139372:20-139372:46',
	'139372:48-139372:173',
	'139372:175-139372:194',
	'139375:1-139375:47',
	'139399:75-139399:89',
	'139399:94-139399:107',
	'139399:109-139399:109',
	'139399:114-139399:117',
	'139399:125-139399:125',
	'139400:1-139400:22',
	'139407:1-139407:976',
	'139407:978-139407:1074',
	'139407:1076-139407:1251',
	'139411:4-139411:67',
	'139411:69-139411:135',
	'139457:1-139457:73',
	'139458:1-139458:103',
	'139458:107-139458:326',
	'139458:329-139458:395',
	'139459:1-139459:2',
	'139459:4-139459:55',
	'139779:4-139779:52',
	'139780:4-139780:75',
	'139781:4-139781:200',
	'139783:4-139783:36',
	'139784:4-139784:71',
	'139786:4-139786:101',
	'139788:4-139788:35',
	'139789:4-139789:112',
	'139790:4-139790:21',
	'139965:209-139965:213',
	'139966:1-139966:51',
	'139967:1-139967:55',
	'139968:1-139968:5',
	'139969:1-139969:78',
	'139971:1-139971:304',
	'139972:1-139972:38',
	'139973:1-139973:118',
	'139974:1-139974:53',
	'139975:1-139975:15',
	'139975:17-139975:54',
	'140058:111-140058:224',
	'140059:1-140059:610',
	'140059:613-140059:1160',
	'140070:1-140070:11',
	'140076:1-140076:60',
	'140124:1-140124:390',
	'140124:392-140124:938',
	'140124:940-140124:1733',
	'140126:1-140126:322',
	'140158:5-140158:404',
	'140159:1-140159:379',
	'140160:1-140160:217',
	'140180:1-140180:55',
	'140181:1-140181:13',
	'140331:222-140331:544',
	'140331:546-140331:614',
	'140331:626-140331:626',
	'140331:635-140331:749',
	'140359:1-140359:35',
	'140359:40-140359:43',
	'140361:1-140361:151',
	'140362:1-140362:354',
	'140379:123-140379:244',
	'140381:16-140381:37',
	'140382:1-140382:128',
	'140382:131-140382:214',
	'140383:1-140383:276',
	'140383:281-140383:447',
	'140383:453-140383:487',
	'140385:1-140385:4',
	'140385:26-140385:30',
	'140385:50-140385:185',
	'140386:1-140386:23',
	'140387:7-140387:137',
	'140388:1-140388:114',
	'140399:1-140399:251',
	'140399:257-140399:283',
	'140399:285-140399:708',
	'140401:1-140401:265',
	'141880:1-141880:200',
	'141881:1-141881:572',
	'141881:575-141881:707',
	'141956:202-141956:445',
	'141956:447-141956:548',
	'141957:1-141957:87',
	'141958:1-141958:34',
	'141959:6-141959:86',
	'141960:1-141960:285',
	'141961:41-141961:117',
	'142035:305-142035:738',
	'142036:1-142036:273',
	'142038:1-142038:185',
	'142038:187-142038:830',
	'142039:1-142039:7',
	'142040:1-142040:341',
	'142076:381-142076:398',
	'142076:403-142076:536',
	'142128:226-142128:239',
	'142129:1-142129:45',
	'142130:1-142130:787',
	'142130:789-142130:789',
	'142130:791-142130:816',
	'142132:1-142132:62',
	'142132:64-142132:85',
	'142132:88-142132:111',
	'142132:113-142132:236',
	'142135:1-142135:128',
	'142135:130-142135:151',
	'142136:1-142136:224',
	'142137:1-142137:477',
	'142137:496-142137:815',
	'142187:207-142187:302',
	'142189:1-142189:252',
	'142191:1-142191:204',
	'142191:206-142191:247',
	'142264:7-142264:62',
	'142265:1-142265:187',
	'142303:138-142303:229',
	'142304:1-142304:11',
	'142305:1-142305:262',
	'142308:1-142308:43',
	'142309:1-142309:108',
	'142309:110-142309:110',
	'142309:116-142309:116',
	'142311:1-142311:638',
	'142312:1-142312:185',
	'142312:204-142312:342',
	'142313:1-142313:149',
	'142413:133-142413:222',
	'142414:8-142414:145',
	'142417:1-142417:8',
	'142418:1-142418:40',
	'142419:1-142419:243',
	'142422:1-142422:1298',
	'142513:1-142513:101',
	'142513:107-142513:107',
	'142514:1-142514:124',
	'142523:140-142523:197',
	'142524:4-142524:306',
	'142524:327-142524:348',
	'142525:1-142525:181',
	'142528:1-142528:976',
	'142530:1-142530:82',
	'142535:1-142535:118',
	'142537:1-142537:114',
	'142537:139-142537:217',
	'142557:118-142557:237',
	'142558:1-142558:429',
	'142657:1-142657:2',
	'142658:1-142658:5',
	'142659:1-142659:64',
	'142660:1-142660:16',
	'142661:1-142661:17',
	'142662:1-142662:186',
	'142663:1-142663:167',
	'142664:1-142664:80',
	'142928:383-142928:578',
	'142928:617-142928:721',
	'142928:724-142928:1046',
	'142933:1-142933:92',
	'142933:194-142933:203',
	'142933:228-142933:1077',
	'142935:1-142935:47',
	'142936:1-142936:16',
	'142953:88-142953:333',
	'142953:342-142953:342',
	'142954:1-142954:240',
	'142970:124-142970:266',
	'142971:1-142971:1701',
	'143004:215-143004:299',
	'143005:1-143005:239',
	'143006:1-143006:117',
	'143007:1-143007:863',
	'143008:113-143008:155',
	'143179:113-143179:169',
	'143181:1-143181:679',
	'143181:681-143181:1055',
	'143187:1-143187:329',
	'143191:1-143191:109',
	'143191:111-143191:164',
	'143191:166-143191:172',
	'143192:1-143192:48',
	'143193:1-143193:76',
	'143318:106-143318:157',
	'143319:8-143319:34',
	'143319:36-143319:60',
	'143320:1-143320:171',
	'143321:1-143321:46',
	'143321:95-143321:95',
	'143322:1-143322:15',
	'143322:30-143322:42',
	'143322:66-143322:118',
	'143322:130-143322:131',
	'143322:139-143322:202',
	'143323:1-143323:49',
	'143323:81-143323:607',
	'143326:1-143326:264',
	'143327:1-143327:228',
	'143328:1-143328:404',
	'143657:210-143657:516',
	'143657:518-143657:2034',
	'143665:1-143665:306',
	'143727:1-143727:135',
	'143727:143-143727:355',
	'143731:1-143731:23',
	'143827:1-143827:219',
	'143827:221-143827:672',
	'143833:1-143833:900',
	'143835:1-143835:284',
	'143953:123-143953:250',
	'143953:271-143953:621',
	'143954:1-143954:187',
	'143955:1-143955:68',
	'143956:1-143956:41',
	'143957:1-143957:52',
	'143957:54-143957:147',
	'143959:1-143959:102',
	'143959:104-143959:124',
	'143960:1-143960:107',
	'143961:1-143961:308',
	'143962:1-143962:352',
	'143962:354-143962:359',
	'144010:1-144010:8',
	'144010:10-144010:73',
	'144010:77-144010:77',
	'144011:1-144011:18',
	'144011:22-144011:398',
	'144086:1-144086:196',
	'144086:198-144086:242',
	'144086:244-144086:265',
	'144086:267-144086:298',
	'144089:1-144089:114',
	'144089:117-144089:161',
	'144089:163-144089:172',
	'144089:174-144089:1605',
	'144089:1607-144089:1693',
	'144112:136-144112:236',
	'144112:238-144112:381',
	'144112:383-144112:437',
	'144112:439-144112:1489',
	'144112:1491-144112:1711',
	'144112:1713-144112:1737',
	'144114:1-144114:79',
	'146428:1-146428:2',
	'146428:11-146428:52',
	'146428:55-146428:92',
	'146430:1-146430:12',
	'146430:18-146430:47',
	'146430:50-146430:62',
	'146430:65-146430:90',
	'146431:1-146431:23',
	'146436:1-146436:532',
	'146437:1-146437:798',
	'146510:1-146510:339',
	'146511:1-146511:63',
	'146511:65-146511:778',
	'146513:1-146513:1',
	'146513:3-146513:15',
	'146514:1-146514:545',
	'146514:548-146514:871',
	'146589:34-146589:248',
	'146644:89-146644:117',
	'146644:119-146644:565',
	'146644:567-146644:867',
	'146644:869-146644:1032',
	'146644:1034-146644:2171',
	'146644:2369-146644:2465',
	'146698:155-146698:180',
	'146698:186-146698:189',
	'146710:1-146710:49',
	'146710:114-146710:115',
	'146710:117-146710:214',
	'146712:1-146712:69',
	'146713:1-146713:48',
	'146713:50-146713:256',
	'146715:1-146715:125',
	'146721:1-146721:6',
	'146724:1-146724:106',
	'146724:109-146724:109',
	'146724:112-146724:150',
	'146724:152-146724:159',
	'146804:111-146804:148',
	'146804:150-146804:520',
	'146804:522-146804:789',
	'146804:791-146804:822',
	'146804:824-146804:905',
	'146807:132-146807:362',
	'146807:365-146807:420',
	'146807:422-146807:469',
	'146944:177-146944:669',
	'147043:161-147043:500',
	'147048:94-147048:484',
	'147114:180-147114:187',
	'147114:227-147114:240',
	'147114:247-147114:667',
	'147115:1-147115:546',
	'147116:1-147116:54'
])
