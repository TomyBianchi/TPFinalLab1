#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///ESTRUCTUTAS
typedef struct
{
	char nombre_ingrediente[20];
	float cantidad;  ///en kg o en litro segun el tipo de ingrediente
	char tipo[20]; /// "liquido" "solido"
	float costo; ///costo por kg o por litro según corresponda
}StockIngrediente;

typedef struct
{
	char nombre_preparacion[20];
	int cantidad; ///por unidad, no hay por peso
}Preparacion;

typedef struct
{
	char nombre_ingrediente[20];
	float cantidad; ///puede ser en litro o en kg
}IngredienteXReceta;

typedef struct
{
	char nombre_preparacion[20];
	IngredienteXReceta ingredientes[20]; ///Puede tener hasta 20 ingredientes
	int cantIngredientes; ///los validos de ingredientes
}Receta;

typedef struct
{
	char nombre_preparacion[20];
	int cantidad; ///por unidad, no hay por peso
 }PreparacionVenta;

typedef struct
{
	char nombre_preparacion[20];
	float precio_venta; ///precio por unidad
}PrecioPreparacion;

typedef struct
{
	char nombre_preparacion[20];
	int cantidad;
}PedidoPreparacion;

typedef struct
{
  int idVenta;
  PedidoPreparacion items_pedido[20]; ///puedo pedir hasta 20 items
  int cantItems; ///los validos del arreglo de items_pedido
  float valor_total; ///valor total a pagar
  int bajaLogica;   ///si esta activo es 1 y si se cancela es 0
}Venta;


///PROTOTIPADO
void menu();                                                                                                	///MENU

void lecturaStock(StockIngrediente[25],int*);                                                               	///Paso 1
void muestraStock(int,StockIngrediente[]);                                                                  	///Paso 1

void demandaPedidos(Preparacion[25],int*);                                                                  	///Paso 2
void muestraDemanda(int, Preparacion[25]);                                                                  	///Paso 2
void lecturaRecetas(Receta[25],int*);                                                                       	///Paso 2
void muestraRecetas(int, Receta[25]);                                                                       	///Paso 2

void disminucionStock(StockIngrediente[25],int,Preparacion[25],int,Receta[25],int,PreparacionVenta[25],int*,float*);   ///Paso 2
void verificacionCocina(int,int,Receta[25],StockIngrediente[25],int*);                                      	///Paso 2 FUNCION DE DISMINUCIONSTOCK
void cocinarLasCosas(int, Receta[25],int,int,StockIngrediente[25],int*,float*);                                    	///Paso 2 FUNCION DE DISMINUCIONSTOCK

void persistenciaTerminadas(PreparacionVenta[25],int);                                                      	///Paso 2
void persistenciaNuevoStock(StockIngrediente[25],int);                                                      	///Paso 2

void listaDePreciosNueva(PrecioPreparacion[25],Receta[25],int,int*);                                        	///Paso 3
void lecturaListaPrecios(PrecioPreparacion [25],int*);                                                       	///Paso 3
void modificarUnPrecio();                                                                                   	///Paso 3
void modificacionEstructuraPrecios(PrecioPreparacion[25],int);                                              	///Paso 3
void mostrarPrecios(PrecioPreparacion[25],int);                                                             	///Paso 3 (muestra y despersistencia de los precios)

void unaVenta(Venta[50],int*,PreparacionVenta[25],int,PrecioPreparacion[25],int,StockIngrediente[25],int);      ///Paso 3
void persistenciaVenta(Venta[50],int);                                                                      	///Paso 3
void mostrarVentas(Venta [50],int);                                                                             ///Paso 3
void anularVenta(PreparacionVenta[25],int,int);                                                                     ///Paso 3

void muestraTerminadas(PreparacionVenta[25],int);                                                               ///Paso 4
float calculoIngresoTotal();                                                                                    ///Paso 4
float gananciaTotal(Venta[50],int,float,StockIngrediente[25],int,Receta[25],int);                                              ///Paso 4


///FUNCIONES
int main()
{
	menu();

	return 0;
}
void menu()                                                                                                              	///MENU
{
	StockIngrediente stock[25];
	int validosStock;

	Preparacion demanda[25];
	int cantDemanda;

	Receta recetas[25];
	int validosRecetas;

	PreparacionVenta terminadas[25];
	int validosTerminadas;

	PrecioPreparacion precios[25];
	int validosPrecios;

	int opcion;
	int opcion2;
	int opcion3;

	Venta ventas[50];
	int validosVentas = 0;

	float ingresoTotal = 0;
	float gastoTotal = 0;
	float ganancia = 0;

	do
	{
    	printf("Seleccione una opcion:\n");
    	printf("Proceso 1: INGRESO DE LA MATERIA PRIMA\n");
    	printf("Proceso 2: PREPARACION DE LA MERCADERIA\n");
    	printf("Proceso 3: VENTAS Y LISTA DE PRECIOS\n");
    	printf("Proceso 4: RESUMEN DEL DIA\n");
    	printf("Presione 0 para salir.\n");
    	fflush(stdin);
    	scanf("%i",&opcion);

    	switch(opcion)
    	{
    	case 1:
        	lecturaStock(stock,&validosStock);
        	break;
    	case 2:
        	demandaPedidos(demanda,&cantDemanda);
        	lecturaRecetas(recetas,&validosRecetas);
        	disminucionStock(stock,validosStock,demanda,cantDemanda,recetas,validosRecetas,terminadas,&validosTerminadas,&gastoTotal);
//        	muestraStock(validosStock,stock);
        	persistenciaTerminadas(terminadas,validosTerminadas);
        	persistenciaNuevoStock(stock,validosStock);
        	break;
    	case 3:
        	printf("\n");
        	printf("Seleccione una opcion:\n");
        	printf("1. Realizar una venta utilizando la lista de precios actual.\n");
        	printf("2. Cancelar una compra.\n");
        	printf("3. Modificar lista de precios.\n");
        	fflush(stdin);
        	scanf("%i",&opcion2);
        	switch(opcion2)
        	{
            case 1:
                lecturaListaPrecios(precios,&validosPrecios);
                unaVenta(ventas,&validosVentas,terminadas,validosTerminadas,precios,validosPrecios,stock,validosStock);
                break;
            case 2:
                anularVenta(terminadas,validosTerminadas,validosVentas);
                break;
        	case 3:
            	printf("\n");
            	printf("Seleccione una opcion:\n");
            	printf("1. Confeccionar una nueva lista de precios.\n");
            	printf("2. Modificar el precio de un producto en la lista actual.\n");
            	fflush(stdin);
            	scanf("%i",&opcion3);
            	if(opcion3==1)
            	{
                	listaDePreciosNueva(precios,recetas,validosRecetas,&validosPrecios);
                	///ESTA ES LA FUNCION PARA HACER LA LISTA DE PRECIOS COMPLETA///
            	}else if(opcion3==2)
            	{
                	modificarUnPrecio();
//                	mostrarPrecios(precios,validosRecetas);
                	///ESTA ES LA FUNCION PARA CAMBIAR EL PRECIO DE UN SOLO PRODUCTO///
            	}
            	break;
            default:
                break;
        	}
        	break;
    	case 4:
            printf("\nREMANENTE DE INGREDIENTES:\n");
            muestraStock(validosStock,stock);

            printf("\nREMANENTE DE PRODUCTOS:\n");
            muestraTerminadas(terminadas,validosTerminadas);

            ingresoTotal = calculoIngresoTotal();
            printf("EL INGRESO TOTAL FUE DE $%.2f\n",ingresoTotal);

            ganancia = ingresoTotal - gastoTotal;
            printf("\nLA GANANCIA FUE DE $%.2f\n",ganancia);

        	break;
    	default:
        	if(opcion!=0)
        	{
            	printf("\nOpcion invalida, vuelva a ingresar una opcion.\n");
        	}
    	}
    	printf("\n");

	}while(opcion!=0);
}
void lecturaStock(StockIngrediente stock[25],int *validosStock)                                                          	///Paso 1
{
	FILE* fp;
	fp=fopen("stockingredientes.bin","rb");

	if(fp!=NULL)
	{
    	fseek(fp,0,SEEK_END);                               	//despersistencia de stockingredientes.bin hacia la variable stock[25]
    	(*validosStock) = ftell(fp)/sizeof(StockIngrediente);
    	fseek(fp,0,SEEK_SET);

    	for(int i = 0; i < (*validosStock); i++)
    	{
        	fread(&stock[i],sizeof(StockIngrediente),1,fp);
    	}
	}
	fclose(fp);

//	muestraStock(*validosStock,stock);  	//muestra el stock

    printf("\nEl Archivo con la materia prima fue leido correctamente\n");
}
void muestraStock(int validosStock,StockIngrediente stock[25])                                                           	///Paso 1 y 4
{
    	for (int i = 0; i < validosStock; i++)                  	//muestra el stock
	{
    	printf("Stock %i:\n",i + 1);
    	printf("Nombre ingrediente  %s\n",stock[i].nombre_ingrediente);
    	printf("Cantidad del ingrediente %.2f en kilos o litros\n",stock[i].cantidad);
    	printf("Tipo de ingrediente: %s\n",stock[i].tipo);
    	printf("Costo por kilo o litro: %.2f\n",stock[i].costo);
    	printf("\n");
	}

}
void demandaPedidos(Preparacion demanda[25],int*cantDemanda)                                                             	///Paso 2
{
	FILE* fp;
	fp=fopen("demanda.bin","rb");

	if(fp!=NULL)
	{
    	fseek(fp,0,SEEK_END);
    	(*cantDemanda) = ftell(fp)/sizeof(Preparacion);           	//despersistencia de demanda.bin hacia la variable demanda
    	fseek(fp,0,SEEK_SET);

    	for(int i = 0; i < (*cantDemanda); i++)
    	{
        	fread(&demanda[i],sizeof(Preparacion),1,fp);
    	}
	}
	fclose(fp);
//	muestraDemanda(*cantDemanda,demanda);   	//muestra la demanda
}
void muestraDemanda(int cantDemanda, Preparacion demanda[25])                                                            	///Paso 2
{
	for (int i = 0; i < cantDemanda; i++)   	//te muestra la demanda
	{
    	printf("Pedido numero %i\n",i + 1);
    	printf("Nombre de preparacion: %s\n",demanda[i].nombre_preparacion);
    	printf("Cantidad: %i\n",demanda[i].cantidad);
    	printf("\n");
	}
}
void lecturaRecetas(Receta recetas[25],int* validosRecetas)                                                              	///Paso 2
{
	FILE* fp;
	fp=fopen("recetas.bin","rb");

	if(fp!=NULL)
	{
    	fseek(fp,0,SEEK_END);
    	(*validosRecetas)=ftell(fp)/sizeof(Receta);         	//despersistencia del archivo recetas.bin hacia la variable recetas[25]
    	fseek(fp,0,SEEK_SET);

    	for(int i = 0; i < (*validosRecetas); i++)
    	{
        	fread(&recetas[i],sizeof(Receta),1,fp);
    	}
	}
	fclose(fp);

//	muestraRecetas(*validosRecetas,recetas);    	//te muestra la receta
}
void muestraRecetas(int validosRecetas, Receta recetas[25])                                                              	///Paso 2
{
	for (int i = 0; i < validosRecetas; i++)    	//Te muestra la receta
	{
    	printf("Receta numero %i\n",i + 1);
    	printf("Nombre de receta: %s\n",recetas[i].nombre_preparacion);

    	for(int j = 0; j < recetas[i].cantIngredientes; j++)
    	{
        	printf("Ingrediente: %s\n",recetas[i].ingredientes[j].nombre_ingrediente);
        	printf("Cantidad: %.2f\n",recetas[i].ingredientes[j].cantidad);
    	}
    	printf("\n");
	}
}
void disminucionStock(StockIngrediente stock[25],int validosStock,Preparacion demanda[25],int cantDemanda,Receta recetas[25],int validosRecetas,PreparacionVenta terminadas[25],int *validosTerminadas,float*gastoTotal) 	///Paso 2    	///Paso 2
{
	int contador;   	//contador que marca cuando se cocino una unidad
	int contadorDos;    	//contador que va a ayudar a ver si se pueden efectuar las recetas


	for(int i = 0; i < cantDemanda; i++)    	//recorremos la demanda
	{
    	contador = 0;   	//reinicio el ontador de cocinados, para pasar a cocinar otro elemento

    	for(int j = 0; j < validosRecetas; j++)       	//recorremos las recetas
    	{
        	if(strcmpi(demanda[i].nombre_preparacion, recetas[j].nombre_preparacion) == 0)       	//encontramos la demanda que tiene el mismo nombre que la receta
        	{
            	for (int t = 0; t < demanda[i].cantidad; t++)       	// for que verifica que no se pase de la demanda necesaria
            	{
                	contadorDos = 0;    	//reinicio el contador para volver a verificar con la siguiente horneada

                    	verificacionCocina(j,validosStock,recetas,stock, &contadorDos);         	//funcion que hace la verificacion de todos los ingredientes para ver si hay cada uno de los ingredientes para cocinar

                    	cocinarLasCosas(contadorDos,recetas,j,validosStock,stock,&contador,gastoTotal);    	//funcion que cocina las cosas de una en una
            	}

            	strcpy(terminadas[i].nombre_preparacion, recetas[j].nombre_preparacion);    	//copiamos el nombre de cada uno a la estructura preparacionVenta
            	terminadas[i].cantidad = contador;                                          	//copiamos la cantidad de cosas que hicimos a preparacionVenta
      	}
    	}

	}
	(*validosTerminadas) = cantDemanda;                 	//validosTerminadas toma el valor de cantDemanda


            printf("\nSe completo la preparacion, lista de productos terminados:\n\n");

	for (int i = 0; i < *validosTerminadas; i++)
	{
    	printf("%s\n",terminadas[i].nombre_preparacion);      	//muestra de nombres y cantidades
    	printf("Cantidad: %i\n",terminadas[i].cantidad);
    	printf("\n");
	}

}
void verificacionCocina(int j,int validosStock,Receta recetas[25],StockIngrediente stock[25],int* contadorDos)           	///Paso 2 FUNCION DE DISMINUCIONSTOCK
{
    	for (int k = 0; k < recetas[j].cantIngredientes; k++)                           	//recorre cada ingrediente de la receta encontrada
        	{
            	for (int p = 0; p < validosStock; p++)                    	//recorre todo el stock de ingredientes, hasta en contrar con el buscado
            	{
                	if (strcmpi(recetas[j].ingredientes[k].nombre_ingrediente, stock[p].nombre_ingrediente)== 0 && stock[p].cantidad - recetas[j].ingredientes[k].cantidad >= 0) 	//encontre el nombre del primer ingrediente y verifico que su resta de 0 o mas
                	{
                    	(*contadorDos)++;  	//el contador se suma cada vez que se puede restar la cantidad necesaria de una receta, al stock
                	}
            	}
        	}
}
void cocinarLasCosas(int contadorDos, Receta recetas[25],int j,int validosStock,StockIngrediente stock[25],int*contador,float*gastoTotal) 	///Paso 2 FUNCION DE DISMINUCIONSTOCK
{

        	if (contadorDos == recetas[j].cantIngredientes) 	//si el contador resulta igual que la cantidad de ingredientes, significa que se puede cocinar una unidad
        	{
            	for(int k = 0; k < recetas[j].cantIngredientes; k++)   	//mismo algoritmo para verificar, solo que aca en vez de verificar, resta uno por uno los ingredientes
            	{
                	for (int p = 0; p < validosStock; p++)
                	{
                        	if (strcmpi(recetas[j].ingredientes[k].nombre_ingrediente, stock[p].nombre_ingrediente)== 0)
                        	{
                            	stock[p].cantidad = stock[p].cantidad - recetas[j].ingredientes[k].cantidad;    	//se resta un ingrediente
                            	(*gastoTotal) = (*gastoTotal) + recetas[j].ingredientes[k].cantidad * stock[p].costo;
                        	}
                	}
            	}
                    	(*contador)++; 	//se cocina una unidad
        	}
}
void persistenciaTerminadas(PreparacionVenta terminadas[25],int validosTerminadas)                                       	///Paso 2 y 4
{
	FILE *fp;
	fp=fopen("terminadas.bin","wb");
	if(fp!=NULL)
	{
    	for(int i = 0; i < validosTerminadas; i++)
    	{
        	fwrite(&terminadas[i],sizeof(PreparacionVenta),1,fp);
    	}
    	fclose(fp);
	}

//	fp=fopen("terminadas.bin","rb");
//	if(fp!=NULL)
//	{
//    	for(int i=0;i<validosTerminadas;i++)                      	//despersistencia para verificar los archivos
//    	{
//        	fread(&terminadas[i],sizeof(PreparacionVenta),1,fp);
//    	}
//    	fclose(fp);
//	}
//
//	for (int i=0;i<validosTerminadas;i++)
//	{
//    	printf("Nombre de preparacion: %s\n",terminadas[i].nombre_preparacion); 	//muestra de las preparaciones para verificar
//    	printf("Cantidad: %i\n",terminadas[i].cantidad);
//    	printf("\n");
//	}
}
void persistenciaNuevoStock(StockIngrediente stock[25],int validosStock)                                                 	///Paso 2
{
	FILE *fp;
	fp = fopen("stockingredientes.bin","wb");
	if(fp!=NULL)
	{
    	for(int i = 0; i < validosStock; i++)
    	{
        	fwrite(&stock[i],sizeof(StockIngrediente),1,fp);
    	}
    	fclose(fp);
	}
}
void listaDePreciosNueva(PrecioPreparacion precios[25],Receta recetas[25],int validosRecetas,int*validosPrecios)        	///Paso 3
{
	(*validosPrecios) = validosRecetas; ///Hay una receta por producto y va a haber un precio por producto tambien


	FILE *fp;
	fp = fopen("precios.bin","wb");

	if(fp!=NULL)
	{
    	for(int i = 0; i < (*validosPrecios) ;i++)
    	{
        	printf("Ingrese el precio unitario de %s\n",recetas[i].nombre_preparacion);
        	fflush(stdin);
        	scanf("%f",&precios[i].precio_venta);
        	strcpy(precios[i].nombre_preparacion,recetas[i].nombre_preparacion);

        	fwrite(&precios[i],sizeof(PrecioPreparacion),1,fp);
    	}
    	fclose(fp);
	}

}
void lecturaListaPrecios(PrecioPreparacion precios[25],int*validosPrecios)                                                  ///Paso 3
{
	FILE* fp;

	fp = fopen("precios.bin","rb");

	if (fp != NULL)
	{
    	fseek(fp,0,SEEK_END);

    	(*validosPrecios) = ftell(fp) / sizeof(PrecioPreparacion);

    	fseek(fp,0,SEEK_SET);

    	for (int i = 0; i < *validosPrecios; i++)
    	{
    	fread(&precios[i],sizeof(PrecioPreparacion),1,fp);
    	}

	fclose(fp);
	}
}
void modificarUnPrecio()                                                                                                   	///Paso 3
{
	char nombre[20];
	PrecioPreparacion precios2[25];
	int validos;
	int i;

	FILE* fp;
	fp = fopen("precios.bin","r+b");
	if(fp!=NULL)
	{
    	fseek(fp,0,SEEK_END);
    	validos = (ftell(fp)/sizeof(PrecioPreparacion));
    	fseek(fp,0,SEEK_SET);

    	for(int j=0;j<validos;j++)
    	{
        	fread(&precios2[j],sizeof(PrecioPreparacion),1,fp);
    	}

    	modificacionEstructuraPrecios(precios2,validos);

    	fseek(fp,0,SEEK_SET);
    	for(int k = 0; k < validos ; k++)
    	{
        	fwrite(&precios2[k],sizeof(PrecioPreparacion),1,fp);
    	}

    	fclose(fp);

	}else
	{
    	printf("No se encontro el archivo con los precios.");
	}
}
void modificacionEstructuraPrecios(PrecioPreparacion precios2[25],int validos)                                           	///Paso 3
{
	char nombre[25];
	printf("Ingrese el nombre del producto al que quiere modificar el precio\n");
	fflush(stdin);
	gets(nombre);

	int i = 0;

	while(i < validos && strcmpi(nombre,precios2[i].nombre_preparacion) != 0)
	{
    	i++;
	}

	if(i < validos)
	{
    	printf("\n Ingrese el precio unitario para %s ",precios2[i].nombre_preparacion);
    	scanf("%f",&precios2[i].precio_venta);
	}else
	{
    	printf("Ese producto no se encuentra en los registros.");
	}
}
void mostrarPrecios(PrecioPreparacion precios[25],int validos)                                                          	///Paso 3 (muestra de precios)
{
	FILE* fp;
	fp = fopen("precios.bin","rb");

	if (fp != NULL)
 	{
    	for(int i = 0; i < validos; i++)
    	{
        	fread(&precios[i],sizeof(PrecioPreparacion),1,fp);
    	}
 	}
 	fclose(fp);


 	for (int j = 0; j < validos; j++)
 	{
     	printf("Nombre: %s\n",precios[j].nombre_preparacion);
     	printf("Precio: %.2f\n",precios[j].precio_venta);
 	}

}
void unaVenta(Venta ventas[50],int*validosVentas,PreparacionVenta terminadas[25],int validosTerminadas,PrecioPreparacion precios[25],int validosPrecios,StockIngrediente stock[25],int validosStock)    	///Paso 3
{
	int i = (*validosVentas);
	int j = 0;
	char continuar;
	int k;
	int p = 0;


	ventas[i].idVenta = 100000 + i; 	//id venta
	do
	{

    	printf("Nombre de preparacion:");  //nombre
    	fflush(stdin);
    	gets(ventas[i].items_pedido[j].nombre_preparacion);

	do
    	{
        	k = 0;
        	while (((strcmpi(ventas[i].items_pedido[j].nombre_preparacion,terminadas[k].nombre_preparacion)!= 0)) && k < validosTerminadas)
        	{
        	k++;
        	}
        	if(k == validosTerminadas)
        	{
        	printf("El producto no existe vuelva a intentar\n");                        	//confirmacion para ver si uso el nombre correctamente
        	fflush(stdin);
        	gets(ventas[i].items_pedido[j].nombre_preparacion);
        	}
    	}
    	while ( k == validosPrecios );

    	printf("\nNOMBRE PUESTO: %s\n",ventas[i].items_pedido[j].nombre_preparacion);


        	printf("Cantidad de la preparacion:");             	//cantidad
        	scanf("%i",&ventas[i].items_pedido[j].cantidad);

        	while (ventas[i].items_pedido[j].cantidad > terminadas[k].cantidad)
        	{
            	ventas[i].items_pedido[j].cantidad = 0;
            	printf("No hay stock, proba con menos cantidad.\n");
            	printf("La cantidad maxima en stock es: %i\n",terminadas[k].cantidad);
            	printf("Cantidad de la preparacion:");             	//cantidad
            	scanf("%i",&ventas[i].items_pedido[j].cantidad);
        	}

        while(strcmpi(terminadas[p].nombre_preparacion,ventas[i].items_pedido[j].nombre_preparacion) !=0 && p < validosTerminadas)
        {
            p++;
        }

        terminadas[p].cantidad = terminadas[p].cantidad - ventas[i].items_pedido[j].cantidad;

       	//una vez que sale del while, conseguis la posicion de k que seria la de precios para evaluar este mismo

    	ventas[i].valor_total = ventas[i].valor_total + ((precios[k].precio_venta) * (ventas[i].items_pedido[j].cantidad));

    	printf("Desea comprar otro producto ? s/n  ");
    	fflush(stdin);
    	scanf("%c",&continuar);
    	j++;
	}
	while (continuar == 's' || continuar == 'S' && j < 20 );

	ventas[i].cantItems = j;    	//es igual a la cantidad de cosas que compro el usuario
	ventas[i].bajaLogica = 1;   	//baja logica


	persistenciaVenta(ventas,*validosVentas);            	///Persistencia de los datos anteriormente cargados

	(*validosVentas)++;

}
void persistenciaVenta(Venta ventas[50],int validosVentas)                                                                  ///Paso 3
{
	FILE*fp;
	fp = fopen("ventas.bin","ab");
	if (fp != NULL)
	{
    	fwrite(&ventas[validosVentas],sizeof(Venta),1,fp);
    	fclose(fp);
	}

    mostrarVentas(ventas,validosVentas);

}
void mostrarVentas(Venta ventas[50],int validosVentas)                                                                      ///Paso 3
{
//    FILE*fp;
    int j = 0;
//
//
//    fp = fopen("ventas.bin","rb");
//
//    if (fp != NULL)
//    {
//        for (int i = 0; i < validosVentas; i++)                     //despersistencia
//        {
//            fread(&ventas[i],sizeof(Venta),1,fp);
//        }
//    }

        printf("\n                 RESUMEN DE TU COMPRA\n");
        j = ventas[validosVentas].cantItems;

        printf("ID: %i\n",ventas[validosVentas].idVenta);
        for (int k = 0; k < j; k++)
        {
        printf("Nombre del producto: %s\n",ventas[validosVentas].items_pedido[k].nombre_preparacion);
        printf("Cantidad: %i\n",ventas[validosVentas].items_pedido[k].cantidad);
        }
        printf("Cantidad total de productos distintos comprados: %i\n",ventas[validosVentas].cantItems);
        printf("Valor total a pagar: %.2f\n",ventas[validosVentas].valor_total);
        printf("Estado de su compra: %i\n",ventas[validosVentas].bajaLogica);

}
void anularVenta(PreparacionVenta terminadas[25],int validosTerminadas,int validosVentas)                                   ///Paso 3
{
    Venta ventas2;
    int encontrado = 0;
    int id;
    FILE* fp;
    fp = fopen("ventas.bin","r+b");

    printf("Ingrese el ID de la venta que desea cancelar \n");
    fflush(stdin);
    scanf("%i",&id);

    if(fp != NULL)
    {
        while(fread(&ventas2,sizeof(Venta),1,fp)!=0)
        {
            if(ventas2.idVenta == id)
            {
                ventas2.bajaLogica=0;
                printf("\nSe anulo la venta\n");
                encontrado++;
                for(int i = 0; i<ventas2.cantItems; i++)
                {
                    int k = 0;
                    while((strcmpi(ventas2.items_pedido[i].nombre_preparacion,terminadas[k].nombre_preparacion) != 0) && k < validosTerminadas)
                    {
                        k++;
                    }
                    terminadas[k].cantidad = terminadas[k].cantidad + ventas2.items_pedido[i].cantidad;
                    fseek(fp,sizeof(Venta)*(-1),SEEK_CUR);
                    fwrite(&ventas2,sizeof(Venta),1,fp);
                }
            }

            if(encontrado == 0)
            {
                printf("El producto no fue encontrado.");
            }
        fclose(fp);
    }
}
}
void muestraTerminadas(PreparacionVenta terminadas[25],int validosTerminadas)                                               ///Paso 4
{
    for (int i = 0; i < validosTerminadas; i++)
	{
    	printf("%s\n",terminadas[i].nombre_preparacion);      	//muestra de nombres y cantidades
    	printf("Cantidad: %i\n",terminadas[i].cantidad);
    	printf("\n");
	}
}
float calculoIngresoTotal()                                                                                                 ///Paso 4
{
    float ingresoTotal = 0;
    Venta ventas2;
    FILE* fp;
    fp = fopen("ventas.bin","rb");

    if(fp != NULL)
    {
        while(fread(&ventas2,sizeof(Venta),1,fp)!=0)
        {
            if(ventas2.bajaLogica == 1)
            {
                ingresoTotal = ingresoTotal + ventas2.valor_total;
            }
        }
        fclose(fp);
    }

    return ingresoTotal;
}

























































