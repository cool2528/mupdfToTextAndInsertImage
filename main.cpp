#include <iostream>
#include <windows.h>
#include <shlwapi.h>
#include "mupdf/pdf.h"
#include "mupdf/fitz/structured-text.h"
#include "Conver.hpp"
#pragma comment(lib,"shlwapi.lib")
class mupdfConvert 
{
	struct fz_text_writer_s
	{
		fz_document_writer super;
		int format;
		fz_stext_options opts;
		fz_stext_page *page;
		fz_output *out;
	};
public:
	mupdfConvert() 
	{

	}
	~mupdfConvert() {

	}
	std::string pdf2Text(const std::string& szPdfPath) {
		std::string strResult;
		if (szPdfPath.empty()) {
			return strResult;
		}
		std::string strPath = Coding_Conver::Conver::AnsiToUtf8(szPdfPath.c_str());
		if (!::PathFileExistsA(strPath.c_str())) {
			return strResult;
		}
		fz_context *ctx = nullptr;
		fz_document *doc = nullptr;
		fz_document_writer* output = nullptr;
		//fz_output* output = nullptr;
		int page_count = 0;
		int page_number = 0;
		ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
		if (!ctx) {
			return strResult;
		}
		fz_try(ctx) {
			fz_register_document_handlers(ctx);
			fz_set_aa_level(ctx, 8);
			fz_set_use_document_css(ctx, 1);
			output = fz_new_document_writer(ctx, "2.txt", "text", "");
			
		}
		fz_catch(ctx)
		{
			fz_drop_context(ctx);
			return strResult;
		}
		/*打开文档*/
		fz_try(ctx) {
			doc = fz_open_document(ctx, strPath.c_str());
			fz_layout_document(ctx, doc, 450, 600, 12);
		}
		fz_catch(ctx) {
			fz_drop_context(ctx);
			return strResult;
		}
		/*计算页数。*/
		fz_try(ctx) {
			page_count = fz_count_pages(ctx, doc);
		}
		fz_catch(ctx)
		{
			fz_drop_document(ctx, doc);
			fz_drop_context(ctx);
			return strResult;
		}
		for (page_number = 0; page_number < page_count; page_number++)
		{
#if 0
			//按道理说这么写应该没问题 可是 偏偏不行
			fz_stext_page* spage = nullptr;
			fz_page* page = nullptr;
			fz_rect rect;
			fz_device *dev = nullptr;
			fz_var(dev);
			ZeroMemory(&rect, sizeof(fz_rect));
			fz_stext_options stext_options;
			fz_var(spage);
			fz_try(ctx) {
				page = fz_load_page(ctx, doc, page_number);
				if (page)
				{
					stext_options.flags = NULL;
					rect = fz_bound_page(ctx, page);
					spage = fz_new_stext_page(ctx, rect);
					dev = fz_new_stext_device(ctx, spage, &stext_options);
					//		fz_enable_device_hints(ctx, dev, FZ_NO_CACHE);
					fz_run_page(ctx, page, dev, fz_identity, NULL);
					if (spage && output) {
						fz_print_stext_page_as_text(ctx, output, spage);
						fz_write_printf(ctx, output, "\r\n");
					}
				}
			}
			fz_always(ctx) {
				if (dev) {
					fz_drop_device(ctx, dev);
					fz_drop_page(ctx, page);
					fz_drop_stext_page(ctx, spage);
					dev = nullptr;
				}
			}
			fz_catch(ctx) {}
#endif
#if 1
			fz_rect mediabox;
			fz_page *page;
			fz_device *dev = NULL;

			page = fz_load_page(ctx, doc, page_number);

			fz_var(dev);

			fz_try(ctx)
			{
				mediabox = fz_bound_page(ctx, page);
				dev = fz_begin_page(ctx, output, mediabox);
				fz_run_page(ctx, page, dev, fz_identity, NULL);
			}
			fz_always(ctx)
			{
				if (dev)
				{
					fz_buffer* buffer = nullptr;
					fz_output* out = nullptr;
					fz_var(buffer);
					fz_var(out);
					buffer = fz_new_buffer(ctx, 8192);
					out = fz_new_output_with_buffer(ctx, buffer);
					if (out)
					{
						fz_text_writer_s *wri = (fz_text_writer_s*)output;
						fz_print_stext_page_as_text(ctx, out, wri->page);
						fz_drop_output(ctx, out);
					}
					if (buffer)
					{
						unsigned char* data = nullptr;
						size_t len = 0;
						len = fz_buffer_storage(ctx, buffer, &data);
						if (data)
						{
							std::string buf;
							buf.assign((char*)data, len);
							strResult += Coding_Conver::Conver::Utf8ToAnsi(buf.c_str());
							
						}
						std::cout << len << std::endl;
						std::cout << strResult << std::endl;
						fz_drop_buffer(ctx, buffer);
					}
				}

				fz_end_page(ctx, output);
				fz_drop_page(ctx, page);
			}
			fz_catch(ctx)
				fz_rethrow(ctx);
#endif
		}
		fz_close_document_writer(ctx, output);
		fz_drop_document_writer(ctx, output);
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);
		return strResult;
	}
	bool PdfInsertImage(const std::string& strFile, const std::string& strImage,const RECT& rect) {
		bool bResult = false;
		std::string strPath = Coding_Conver::Conver::AnsiToUtf8(strFile.c_str());
		if (!::PathFileExistsA(strPath.c_str()) || !::PathFileExistsA(strImage.c_str())) {
			return bResult;
		}
		fz_context *ctx = nullptr;
		fz_document *doc = nullptr;
		ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
		if (!ctx) {
			return bResult;
		}
		fz_try(ctx) {
			fz_register_document_handlers(ctx);
			fz_set_aa_level(ctx, 8);
			fz_set_use_document_css(ctx, 1);

		}
		fz_catch(ctx)
		{
			fz_drop_context(ctx);
			return bResult;
		}
		/*打开文档*/
		fz_try(ctx) {
			doc = fz_open_document(ctx, strPath.c_str());
			fz_layout_document(ctx, doc, 450, 600, 12);
		}
		fz_catch(ctx) {
			fz_drop_context(ctx);
			return bResult;
		}
		/*读入图片*/
		
		fz_image* image = nullptr;
		pdf_document *pdf;
		const char *_imgname = "2.png";
		pdf_obj *resources, *xobject, *ref;
		const char *templates = " q %g %g %g %g %g %g cm /%s Do Q ";
		pdf_page *page = pdf_page_from_fz_page(ctx, fz_load_page(ctx, doc,0));
		fz_rect pdfRect = pdf_bound_page(ctx, page);
		/*fz_matrix 的 a 和 d 控制 高宽  e和 f 控制 左边 顶边 
		没整明白 这个 fz_matrix 大致是这样吧 要是有研究明白的
		也麻烦告知我下
		*/
		fz_matrix mat = fz_make_matrix(rect.right - rect.left, 0, 0, rect.bottom - rect.top, rect.left,20);
		fz_try(ctx) {
			image = fz_new_image_from_file(ctx, strImage.c_str());
		}fz_always(ctx) {
			fz_pixmap* pixmap = nullptr;
			fz_pixmap* pixmap2 = nullptr;
			fz_image* mask = nullptr;
			fz_image* zImage = nullptr;
			if (image)
			{
				fz_color_params color_params = { 0 };
				pixmap = fz_get_pixmap_from_image(ctx, image, NULL, NULL, 0, 0);
				if (pixmap)
				{
					if (pixmap->alpha == 1)
					{
						pixmap2 = fz_convert_pixmap(ctx, pixmap, NULL, NULL, NULL, &color_params, 1);
						pixmap2->alpha = 0;
						pixmap2->colorspace = fz_keep_colorspace(ctx, fz_device_gray(ctx));
						mask = fz_new_image_from_pixmap(ctx, pixmap2, NULL);
						zImage = fz_new_image_from_pixmap(ctx, pixmap, mask);
						fz_drop_image(ctx, image);
						image = zImage;
						zImage = nullptr;
					}
					else
					{
						if (pixmap->alpha == 0)
							image = fz_new_image_from_pixmap(ctx, pixmap, NULL);
						else
						{   // pixmap has alpha: create an SMask
							pixmap2 = fz_convert_pixmap(ctx, pixmap, NULL, NULL, NULL, &color_params, 1);
							pixmap2->alpha = 0;
							pixmap2->colorspace = fz_keep_colorspace(ctx, fz_device_gray(ctx));
							mask = fz_new_image_from_pixmap(ctx, pixmap2, NULL);
							image = fz_new_image_from_pixmap(ctx, pixmap, mask);
						}
					}
				}
				pdf = page->doc;
				resources = pdf_dict_get_inheritable(ctx, page->obj, PDF_NAME(Resources));
				xobject = pdf_dict_get(ctx, resources, PDF_NAME(XObject));
				if (!xobject) //还没有XObject，创建一个
				{
					xobject = pdf_new_dict(ctx, pdf, 10);
					pdf_dict_putl_drop(ctx, page->obj, xobject, PDF_NAME(Resources), PDF_NAME(XObject), NULL);
				}
				ref = pdf_add_image(ctx, pdf, image,0);
				pdf_dict_puts(ctx, xobject, _imgname, ref); //更新XObject
				auto nres = fz_new_buffer(ctx, 50);
				fz_append_printf(ctx, nres, templates,
					mat.a, mat.b, mat.c, mat.d, mat.e, mat.f,
					_imgname);
				//unsigned char* data = nullptr;
				//size_t len = 0;
				//len = fz_buffer_storage(ctx, nres, &data);
				JM_insert_contents(ctx, pdf, page->obj, nres, 1);
				fz_drop_buffer(ctx, nres);
				pdf_write_options opts = {0};
				opts.do_incremental = 0;
				opts.do_ascii = 0;
				opts.do_compress = 0;
				opts.do_compress_images = 0;
				opts.do_compress_fonts = 0;
				opts.do_decompress = 0;
				opts.do_garbage = 0;
				opts.do_pretty = 0;
				opts.do_linear = 0;
				opts.do_clean = 0;
				opts.do_sanitize = 0;
				pdf_document *pdf = pdf_specifics(ctx, doc);
				JM_embedded_clean(ctx, pdf);
				pdf_save_document(ctx, pdf, "5.pdf", &opts);
				pdf->dirty = 0;
				bResult = true;

			}
			fz_drop_pixmap(ctx, pixmap2);
			fz_drop_pixmap(ctx, pixmap);
			fz_drop_image(ctx, mask);
			fz_drop_image(ctx, image);
		}
		fz_catch(ctx) {
			fz_drop_document(ctx,doc);
			fz_drop_context(ctx);
			return bResult;
		}
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);
		return bResult;
	}
private:
void JM_embedded_clean(fz_context *ctx, pdf_document *pdf)
	{
		pdf_obj *root = pdf_dict_get(ctx, pdf_trailer(ctx, pdf), PDF_NAME(Root));

		// remove any empty /Collection entry
		pdf_obj *coll = pdf_dict_get(ctx, root, PDF_NAME(Collection));
		if (coll && pdf_dict_len(ctx, coll) == 0)
			pdf_dict_del(ctx, root, PDF_NAME(Collection));

		pdf_obj *efiles = pdf_dict_getl(ctx, root,
			PDF_NAME(Names),
			PDF_NAME(EmbeddedFiles),
			PDF_NAME(Names),
			NULL);
		if (efiles)  // make sure embedded files get displayed by viewers
		{
			pdf_dict_put_name(ctx, root, PDF_NAME(PageMode), "UseAttachments");
		}
		return;
	}
	int JM_insert_contents(fz_context *ctx, pdf_document *pdf,
		pdf_obj *pageref, fz_buffer *newcont, int overlay) {
		int xref = 0;
		fz_try(ctx)
		{
			pdf_obj *contents = pdf_dict_get(ctx, pageref, PDF_NAME(Contents));
			pdf_obj *newconts = pdf_add_stream(ctx, pdf, newcont, NULL, 0);
			xref = pdf_to_num(ctx, newconts);
			if (pdf_is_array(ctx, contents))
			{
				if (overlay)               // append new object
					pdf_array_push_drop(ctx, contents, newconts);
				else                       // prepend new object
					pdf_array_insert_drop(ctx, contents, newconts, 0);
			}
			else                           // make new array
			{
				pdf_obj *carr = pdf_new_array(ctx, pdf, 5);
				if (overlay)
				{
					if (contents) pdf_array_push(ctx, carr, contents);
					pdf_array_push_drop(ctx, carr, newconts);
				}
				else
				{
					pdf_array_push_drop(ctx, carr, newconts);
					if (contents) pdf_array_push(ctx, carr, contents);
				}
				pdf_dict_put_drop(ctx, pageref, PDF_NAME(Contents), carr);
			}
		}
		fz_catch(ctx) fz_rethrow(ctx);
		return xref;
	}
};
void test() {
	mupdfConvert mupdfs;
	RECT rect = { 0,0,80,80 };
	mupdfs.PdfInsertImage(R"(D:\download\xpdf-tools-win-4.01.01\bin32\1.pdf)", R"(D:\download\xpdf-tools-win-4.01.01\bin32\2.png)", rect);
}
int main() {
	test();
	system("pause");
	return 0;
}