# BTree-Dictionary

Thư viện dict.h có:


-Kiểu biến:

BT_key là khóa/từ để tra

BT_val là giá trị/nghĩa trả về

BT_item là thằng chứa cả key và val (có thể sẽ sửa lại, chưa chắc cấu trúc như thế nào là hợp lí, không nên dùng nhiều)

BT_dict là con trỏ của kiểu biến từ điển (btree_dict) để định nghĩa lại biến ở file dict.c (Nếu không phải là con trỏ thì lỗi tùm lum)


-Hàm:

init_dtn() là hàm đọc data từ điển.

dtn_insert() là hàm cho từ mới vào từ điển. Trả lại 0 nếu insert thành công.

dtn_suggest() là hàm gợi ý từ tra, trả lại mảng key chứa key gốc, đặt biến size là kích cỡ của mảng và kích cỡ của mảng max sẽ là max_size (người gọi hàm chọn).

dtn_search() là hàm tra hẳn từ (có thể chuẩn bị lỗi thời do dtn_suggest() gần giống). Trả lại NULL nếu không tìm thấy.

dtn_delete() là hàm xóa từ. Trả lại NULL nếu không tìm thấy phần tử để xóa. Nếu tìm thấy thì trả lại từ và nghĩa đã bị xóa.

dtn_free() là hàm xóa hết từ trong từ điển

Yêu cầu:

Làm lại thằng dict.c dùng Btree :)

Làm dtn_insert() rồi dtn_delete() và dtn_free() rồi nghĩ cách làm thằng dtn_suggest() bằng Btree trước rồi mới làm dtn_search()

Làm thêm test cho basic_test.c (File này chủ yếu chứa test mấy cái hàm gốc thực hiện đúng không thôi, muốn test mấy lỗi khó tìm hơn thì tạo file test khác, không được xóa test cũ!)
