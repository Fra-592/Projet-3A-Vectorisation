#ifndef _IO_H
	#define _IO_H
	
	// Liste les images disponibles dans le dossier img/
	int list_images(void);

	// Vérifie l'existence du fichier et l'intégrité des headers
	int is_valid_img(const char *imgName);

	// Renvoie la largeur de l'image telle qu'indiquée dans le header BMP
	int get_image_width(const char *imgName);

	// Renvoie la hauteur de l'image telle qu'indiquée dans le header BMP
	int get_image_height(const char *imgName);

	// Renvoie une matrice d'entiers représentant l'image, 1 = noir, 0 = blanc
	// La transformation en noir et blanc se fait à la volée le plus simplement du monde
	// Le blanc reste blanc, le reste devient noir
	int get_image_pixels(const char *imgName, int **image);
#endif
