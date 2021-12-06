//
//
//    Fonctions de conversion de type
//
//
//
//      String IpAddress2String(const IPAddress& ipAddress) {
//      String float_to_string(float x, int e, int d )
//      String unsignedint_to_string( int x, int l)
//
//



String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}

String float_to_string(float x, bool signe, int e, int d ) {
  //  Convertit un float signé en une chaine de caractères
  //      signe : rajoute le signe si true ne le rajoute pas si false
  //      x : la valeur à convertir
  //      e : le nombre de digits de la partie entière
  //      d : le nb de digits de la partie décimale
  //          si ces deux dernières valeurs sont supérieures aux digits nécessaires, on complémente avec des 0
  //      La fonction supprime la virgule de la chaine
  //
  //        ex. : (-85.48, 4, 3)  donne "-0085480"
  //        ex. : (85.48,1,1) donne "+8548"
  //
  // x float à convertir
  // e nb de caractéres significatifs de la partie entière (0 à 
  String res="";
  if (signe==true) {
    res="+";
    if (x<0) {
        x=-x;
        res="-";
    }
  }
  String s = String(x,d);
  Serial.println ("=> " + s);
  int k=s.indexOf(".");
  Serial.println ("k : "+String(k));

  String ee="";
  if (k!=-1) {
    ee=s.substring(0,k); // prend les caractères avant la virgule
  } else {
    ee=s;
  }

  for (int i=0; i<e; i++) {
    ee="0"+ee;
  }
  res = res + ee.substring(ee.length()-e);
  if (k!=-1) {
    res+= s.substring(k+1);
  }
  return res;
}

String unsignedint_to_string( int x, int l) {
  //  Convertit un entier non signé en une chaine de caractères
  //  x : la valeur à convertir en chaine
  //  l : la longueur de la chaine
  //          si l <= 0 => l'entier est convertit tel quel sans ajout de zéros significatifs (ex. : (4265,0) donne "4265")
  //          si l <= nb de digits nécessaires : idem ci-dessus (ex. : (4265,3) donne "4265" )
  //          si l > nb de digits nécessaires : adjonction de "0" significatifs en tête de chaine (ex. : (45,5) donne "00045")
  String res = String(x);
  int ll = res.length();
  if (l > ll) {
    for (int i = 0; i < l - ll; i++) {
      res = "0" + res;
    }
  }
  return res;
}
