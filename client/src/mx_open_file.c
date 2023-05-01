#include "../inc/uchat.h"

#define JPEG_SAVE_QUALITY "40"

void scale_image(GtkWidget *imagetemp, int width, int height) {
  GdkPixbuf *pixbuf;
  GdkPixbuf *scaled_pixbuf;

  pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(imagetemp));

  scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);

  gtk_image_set_pixel_size(GTK_IMAGE(imagetemp), 200);

  gtk_image_set_from_pixbuf(GTK_IMAGE(imagetemp), scaled_pixbuf);
}

void on_PhotoReg_clicked(GtkWidget *widget, gpointer data){
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new("Open File", NULL, action, "_Cancel",
                                       GTK_RESPONSE_CANCEL, "_Open",
                                       GTK_RESPONSE_ACCEPT, NULL);
    GtkFileFilter *filter;
  filter = gtk_file_filter_new();
  gtk_file_filter_new();
  gtk_file_filter_add_pixbuf_formats(filter);
  gtk_file_filter_set_name(filter, "images");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT) {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    
    
    /*int num;
    for(int i = mx_strlen(filename) - 1; i > 0; i--) {
      if (filename[i] == '.') num = i + 1;
    }
    char *check = malloc(sizeof(char) * (mx_strlen(filename) - num));
    for (int i = 0; filename[num + i] != '\0'; i++) {
      check[i] = filename[num + i];
    }
    if (!mx_strcmp(check, "jpg") || !mx_strcmp(check, "jpeg") || !mx_strcmp(check, "png")) {
      return;
    }
    else {*/
      long len = 0;
      filename = mx_compress_image(filename);
      char *bytebuf = mx_read_full_file(filename, &len);
      binaryb64 *b64buf = malloc(Base64encode_len(len) + 1);
      b64buf[Base64encode_len(len) + 1] = 0;
      Base64encode(b64buf, bytebuf, len);
      free(bytebuf);
      if (strcmp(reg_form.avathar, "") == 0) {
        reg_form.avathar = b64buf;
      }
      else {
        free(reg_form.avathar);
        reg_form.avathar = b64buf;
      } 
      

      gtk_image_set_from_file(GTK_IMAGE(imageReg), filename);
      scale_image(imageReg, 50, 50);
      mx_delete_tmp_file(filename);
    //}
  }
  gtk_widget_destroy(dialog);
}

//creates temporary file and writes to it from buf len bytes
//returns name of created file
char *mx_make_tmp_file(char *buff, size_t len) {
    char tmp[32];
    memset(tmp, 0, 32);
    mx_strcpy(tmp, "/tmp/uchat_temp.XXXXXXX");
    char *name = mktemp(tmp);
    int fd = open(name, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    write(fd, buff, len);
    close(fd);
    return strdup(tmp);
}

//returns name of compressed image file
char *mx_compress_image(char *file) {
    GdkPixbuf *pb = gdk_pixbuf_new_from_file(file, NULL);
    char *newFile = mx_make_tmp_file("", 0);
    gdk_pixbuf_save(pb, newFile, "jpeg", NULL, "quality", JPEG_SAVE_QUALITY, NULL);
    return newFile;
}

//delete tmp file
void mx_delete_tmp_file(char *filename) {
    remove(filename);
}

char *mx_read_full_file(char *file, long *len) {
    FILE *f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    long i = ftell(f);
    *len = i;
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(i);
    fread(buf, i, 1, f);
    fclose(f);
    return buf;
}



char *mx_b64_to_file(char *b64str) {
    size_t declen = Base64decode_len(b64str);
		char *plain = malloc(declen);
		memset(plain, 0, declen);
		Base64decode(plain, b64str);
		char *file = mx_make_tmp_file(plain, declen);
		//log_info(file);
		free(plain);
    return file;
}

