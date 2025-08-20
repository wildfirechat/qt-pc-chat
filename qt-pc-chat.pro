QT       += core gui network svg xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG+=sdk_no_version_check
QMAKE_CXXFLAGS_RELEASE += -O2
PKGCONFIG += xcb xcb-util

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


DEFINES += RAPIDJSON_HAS_STDSTRING=1

DEFINES += \
        USE_SPDLOG_ \
        NZENTAO_VER_

SOURCES += \
    singleinstance.cpp \
    src/appservice/appservice.cpp \
    src/config.cpp \
    src/pages/channeldetail/channeldetailwidget.cpp \
    src/pages/chatbox/atmemberdelegate.cpp \
    src/pages/chatbox/atmemberlistview.cpp \
    src/pages/chatbox/cell/basecell.cpp \
    src/pages/chatbox/cell/callcell.cpp \
    src/pages/chatbox/cell/cardcell.cpp \
    src/pages/chatbox/cell/compositecell.cpp \
    src/pages/chatbox/cell/filecell.cpp \
    src/pages/chatbox/cell/headercell.cpp \
    src/pages/chatbox/cell/headerlesscell.cpp \
    src/pages/chatbox/cell/imagecell.cpp \
    src/pages/chatbox/cell/informationcell.cpp \
    src/pages/chatbox/cell/linkcell.cpp \
    src/pages/chatbox/cell/locationcell.cpp \
    src/pages/chatbox/cell/notificationcell.cpp \
    src/pages/chatbox/cell/recallcell.cpp \
    src/pages/chatbox/cell/richnotificationcell.cpp \
    src/pages/chatbox/cell/stickercell.cpp \
    src/pages/chatbox/cell/textcell.cpp \
    src/pages/chatbox/cell/unknowncell.cpp \
    src/pages/chatbox/cell/videocell.cpp \
    src/pages/chatbox/cell/voicecell.cpp \
    src/pages/chatbox/chatdelegate.cpp \
    src/pages/chatbox/chatmodel.cpp \
    src/pages/chatbox/emojdialog.cpp \
    src/pages/chatbox/inputtextedit.cpp \
    src/pages/chatbox/messagedata.cpp \
    src/pages/chatbox/settings/conversationsettingswidget.cpp \
    src/pages/chatlist/chatlistdelegate.cpp \
    src/pages/chatlist/chatlistmodel.cpp \
    src/pages/contactlist/contactitemdelegate.cpp \
    src/pages/contactlist/contactlistmodel.cpp \
    src/pages/friendselectiondialog/friendselectabledelegate.cpp \
    src/pages/friendselectiondialog/friendselectablemodel.cpp \
    src/pages/friendselectiondialog/friendselectiondialog.cpp \
    src/pages/friendrequestdetail/friendrequestdetailwidget.cpp \
    src/pages/friendselectiondialog/selectedfrienddelegate.cpp \
    src/pages/groupdetail/groupdetailwidget.cpp \
    src/pages/login/codelineedit.cpp \
    src/pages/login/loginwidget.cpp \
    main.cpp \
    src/pages/chatbox/chatdetailwidget.cpp \
    src/pages/chatlist/chatlistwidget.cpp \
    src/pages/userdetail/contactdetailwidget.cpp \
    src/pages/contactlist/contactlistwidget.cpp \
    src/pages/mainwindow/contentwidget.cpp \
    src/pages/mainwindow/detailwidget.cpp \
    src/pages/mainwindow/emptydetailwidget.cpp \
    src/pages/favorite/favoritedetailwidget.cpp \
    src/pages/favorite/favoritelistwidget.cpp \
    src/pages/mainwindow/mainwindow.cpp \
    src/pages/navigationwidget/navigationwidget.cpp \
    src/pages/settings/settingsdialog.cpp \
    src/pages/settings/settingswidget.cpp \
    src/pages/tag/tagdetailwidget.cpp \
    src/pages/tag/taglistwidget.cpp \
    src/pages/userdialog/userinfodialog.cpp \
    src/pages/utility/avataritemdelegate.cpp \
    src/pages/utility/avatarwidget.cpp \
    src/pages/utility/badgebutton.cpp \
    src/pages/utility/defaultportraitfactory.cpp \
    src/pages/utility/loadingdialog.cpp \
    src/tools/imageloader.cpp \
    src/tools/timeformatter.cpp \
    src/vendor/ezviewer/imageviewer_config.cpp \
    src/vendor/ezviewer/imageviewer_mainwindow.cpp \
    src/vendor/qrcode/QrCodeGenerator.cpp \
    src/vendor/qrcode/qrcodegen.cpp \
    src/wfc/client/ChatClient.cpp \
    src/wfc/message/AddGroupMemberNotificationContent.cpp \
    src/wfc/message/CallStartMessageContent.cpp \
    src/wfc/message/ChangeGroupNameNotificationContent.cpp \
    src/wfc/message/ChangeGroupPortraitNotificationContent.cpp \
    src/wfc/message/CreateGroupNotificationContent.cpp \
    src/wfc/message/DismissGroupNotificationContent.cpp \
    src/wfc/message/FileMessageContent.cpp \
    src/wfc/message/GroupJoinTypeNotificationContent.cpp \
    src/wfc/message/GroupMuteNotificationContent.cpp \
    src/wfc/message/GroupPrivateChatNotificationContent.cpp \
    src/wfc/message/GroupSetManagerNotificationContent.cpp \
    src/wfc/message/ImageMessageContent.cpp \
    src/wfc/message/KickoffGroupMemberNotificaionContent.cpp \
    src/wfc/message/LocationMessageContent.cpp \
    src/wfc/message/MarkUnreadMessageContent.cpp \
    src/wfc/message/MediaMessageContent.cpp \
    src/wfc/message/Message.cpp \
    src/wfc/message/MessageContent.cpp \
    src/wfc/message/MessagePayload.cpp \
    src/wfc/message/ModifyGroupAliasNotificationContent.cpp \
    src/wfc/message/NotificationMessageContent.cpp \
    src/wfc/message/PTextMessageContent.cpp \
    src/wfc/message/QuitGroupNotificationContent.cpp \
    src/wfc/message/RecallMessageContent.cpp \
    src/wfc/message/RejectJoinGroupNotificationContent.cpp \
    src/wfc/message/SoundMessageContent.cpp \
    src/wfc/message/StickerMessageContent.cpp \
    src/wfc/message/TextMessageContent.cpp \
    src/wfc/message/TipNotificationContent.cpp \
    src/wfc/message/TransferGroupOwnerNotificationContent.cpp \
    src/wfc/message/TypingMessageContent.cpp \
    src/wfc/message/UnknownMessageContent.cpp \
    src/wfc/message/VideoMessageContent.cpp \
    src/wfc/model/ChannelInfo.cpp \
    src/wfc/model/ChatroomInfo.cpp \
    src/wfc/model/Conversation.cpp \
    src/wfc/model/ConversationInfo.cpp \
    src/wfc/model/ConversationSearchInfo.cpp \
    src/wfc/model/FriendRequest.cpp \
    src/wfc/model/GroupInfo.cpp \
    src/wfc/model/GroupSearchInfo.cpp \
    src/wfc/model/UnreadCount.cpp \
    src/wfc/model/UserInfo.cpp \
    src/wfc/model/UserSettingEntry.cpp \
    src/wfc/utility/JsonTools.cpp \
    src/wfc/utility/Serializable.cpp \
    src/vendor/zenshot/commands/addcommand.cpp \
    src/vendor/zenshot/commands/deletecommand.cpp \
    src/vendor/zenshot/commands/locatorcommand.cpp \
    src/vendor/zenshot/commands/movecommand.cpp \
    src/vendor/zenshot/commands/propscommand.cpp \
    src/vendor/zenshot/commands/textcontentcommand.cpp \
    src/vendor/zenshot/config/xmlconfig.cpp \
    src/vendor/zenshot/controls/colorbutton.cpp \
    src/vendor/zenshot/controls/colorwidget.cpp \
    src/vendor/zenshot/controls/pencombobox.cpp \
    src/vendor/zenshot/controls/penitemwidget.cpp \
    src/vendor/zenshot/controls/penrenderer.cpp \
    src/vendor/zenshot/controls/sizebutton.cpp \
    src/vendor/zenshot/controls/sizeinputwidget.cpp \
    src/vendor/zenshot/controls/sizewidget.cpp \
    src/vendor/zenshot/controls/textwidget.cpp \
    src/vendor/zenshot/controls/textwidgetimpl.cpp \
    src/vendor/zenshot/core/command.cpp \
    src/vendor/zenshot/core/gparams.cpp \
    src/vendor/zenshot/core/gscale.cpp \
    src/vendor/zenshot/core/handle.cpp \
    src/vendor/zenshot/core/locator.cpp \
    src/vendor/zenshot/core/pointshape.cpp \
    src/vendor/zenshot/core/propsbar.cpp \
    src/vendor/zenshot/core/rectshape.cpp \
    src/vendor/zenshot/core/screenlist.cpp \
    src/vendor/zenshot/core/shape.cpp \
    src/vendor/zenshot/core/store.cpp \
    src/vendor/zenshot/core/tool.cpp \
    src/vendor/zenshot/core/useroper.cpp \
    src/vendor/zenshot/core/utils.cpp \
    src/vendor/zenshot/handles/rectshapehandle.cpp \
    src/vendor/zenshot/locators/pointlocator.cpp \
    src/vendor/zenshot/locators/rectlocator.cpp \
    src/vendor/zenshot/preview/zentaobug.cpp \
    src/vendor/zenshot/preview/zentaopreview.cpp \
    src/vendor/zenshot/preview/zentaosubmit.cpp \
    src/vendor/zenshot/preview/zentaotips.cpp \
    src/vendor/zenshot/properties/arrowbar.cpp \
    src/vendor/zenshot/properties/curvebar.cpp \
    src/vendor/zenshot/properties/ellipsebar.cpp \
    src/vendor/zenshot/properties/linebar.cpp \
    src/vendor/zenshot/properties/mosaicbar.cpp \
    src/vendor/zenshot/properties/rectanglebar.cpp \
    src/vendor/zenshot/properties/textbar.cpp \
    src/vendor/zenshot/screen/handletool.cpp \
    src/vendor/zenshot/screen/hovertool.cpp \
    src/vendor/zenshot/screen/movetool.cpp \
    src/vendor/zenshot/screen/shotarea.cpp \
    src/vendor/zenshot/screen/shotarea/areacreatetool.cpp \
    src/vendor/zenshot/screen/shotarea/areahandle.cpp \
    src/vendor/zenshot/screen/shotarea/areahandletool.cpp \
    src/vendor/zenshot/screen/shotarea/arealocator.cpp \
    src/vendor/zenshot/screen/shotarea/areamovetool.cpp \
    src/vendor/zenshot/screen/textassist.cpp \
    src/vendor/zenshot/screen/toolbar.cpp \
    src/vendor/zenshot/screen/workspace.cpp \
    src/vendor/zenshot/setting/settingdlg.cpp \
    src/vendor/zenshot/setting/zentaosetting.cpp \
    src/vendor/zenshot/setting/ztsettingdetail.cpp \
    src/vendor/zenshot/setting/ztsettinglist.cpp \
    src/vendor/zenshot/setting/ztsettinglistitem.cpp \
    src/vendor/zenshot/shapes/arrow.cpp \
    src/vendor/zenshot/shapes/curve.cpp \
    src/vendor/zenshot/shapes/ellipse.cpp \
    src/vendor/zenshot/shapes/line.cpp \
    src/vendor/zenshot/shapes/mosaic.cpp \
    src/vendor/zenshot/shapes/rectangle.cpp \
    src/vendor/zenshot/shapes/text.cpp \
    src/vendor/zenshot/spdlogwrapper.cpp \
    src/vendor/zenshot/starter.cpp \
    src/vendor/zenshot/starterui.cpp \
    src/vendor/zenshot/stores/localstore.cpp \
    src/vendor/zenshot/stores/memorystore.cpp \
    src/vendor/zenshot/tools/arrowcreatetool.cpp \
    src/vendor/zenshot/tools/curvecreatetool.cpp \
    src/vendor/zenshot/tools/ellipsecreatetool.cpp \
    src/vendor/zenshot/tools/linecreatetool.cpp \
    src/vendor/zenshot/tools/mosaiccreatetool.cpp \
    src/vendor/zenshot/tools/rectcreatetool.cpp \
    src/vendor/zenshot/tools/textcreatetool.cpp \
    src/vendor/zenshot/widget.cpp \
    src/vendor/zenshot/zentaologic.cpp \
    src/vendor/ezviewer/imageviewer.cpp \
    src/vendor/ezviewer/imagewrapper.cpp \
    src/vendor/ezviewer/imagefactory.cpp \
    src/vendor/ezviewer/picmanager.cpp \
    src/vendor/ezviewer/settingdialog.cpp \
    src/vendor/ezviewer/ui/contralbar.cpp \
    src/vendor/ezviewer/ui/commonsetting.cpp \
    src/vendor/ezviewer/ui/shortcutsetting.cpp \
    src/vendor/ezviewer/tools/floatframe.cpp \
    src/vendor/ezviewer/tools/fileassoc.cpp \
    src/vendor/ezviewer/tools/osrelated.cpp \
    src/vendor/ezviewer/tools/toolkit.cpp \
    src/vendor/ezviewer/tools/tooltip.cpp \
    src/vendor/ezviewer/tools/actionmanager.cpp \
    src/vendor/ezviewer/tools/velocitytracker.cpp \
    src/vendor/ezviewer/3rdparty/qxmlputget.cpp \
    src/vendor/ezviewer/3rdparty/exif.cpp \
    src/vendor/ezviewer/tools/imageheader.cpp \
    src/vendor/ezviewer/3rdparty/jpgqguess.cpp


HEADERS += \
    singleinstance.h \
    src/appservice/appservice.h \
    src/config.h \
    src/pages/channeldetail/channeldetailwidget.h \
    src/pages/chatbox/atmemberdelegate.h \
    src/pages/chatbox/atmemberlistview.h \
    src/pages/chatbox/cell/basecell.h \
    src/pages/chatbox/cell/callcell.h \
    src/pages/chatbox/cell/cardcell.h \
    src/pages/chatbox/cell/compositecell.h \
    src/pages/chatbox/cell/filecell.h \
    src/pages/chatbox/cell/headercell.h \
    src/pages/chatbox/cell/headerlesscell.h \
    src/pages/chatbox/cell/imagecell.h \
    src/pages/chatbox/cell/informationcell.h \
    src/pages/chatbox/cell/linkcell.h \
    src/pages/chatbox/cell/locationcell.h \
    src/pages/chatbox/cell/notificationcell.h \
    src/pages/chatbox/cell/recallcell.h \
    src/pages/chatbox/cell/richnotificationcell.h \
    src/pages/chatbox/cell/stickercell.h \
    src/pages/chatbox/cell/textcell.h \
    src/pages/chatbox/cell/unknowncell.h \
    src/pages/chatbox/cell/videocell.h \
    src/pages/chatbox/cell/voicecell.h \
    src/pages/chatbox/chatdelegate.h \
    src/pages/chatbox/chatmodel.h \
    src/pages/chatbox/emojdialog.h \
    src/pages/chatbox/inputtextedit.h \
    src/pages/chatbox/messagedata.h \
    src/pages/chatbox/settings/conversationsettingswidget.h \
    src/pages/chatlist/chatlistdelegate.h \
    src/pages/chatlist/chatlistmodel.h \
    src/pages/contactlist/contactitemdelegate.h \
    src/pages/contactlist/contactlistmodel.h \
    src/pages/friendselectiondialog/friendselectabledelegate.h \
    src/pages/friendselectiondialog/friendselectablemodel.h \
    src/pages/friendselectiondialog/friendselectiondialog.h \
    src/pages/friendrequestdetail/friendrequestdetailwidget.h \
    src/pages/friendselectiondialog/selectedfrienddelegate.h \
    src/pages/groupdetail/groupdetailwidget.h \
    src/pages/login/codelineedit.h \
    src/pages/login/loginwidget.h \
    src/pages/chatbox/chatdetailwidget.h \
    src/pages/chatlist/chatlistwidget.h \
    src/pages/userdetail/contactdetailwidget.h \
    src/pages/contactlist/contactlistwidget.h \
    src/pages/mainwindow/contentwidget.h \
    src/pages/mainwindow/detailwidget.h \
    src/pages/mainwindow/emptydetailwidget.h \
    src/pages/favorite/favoritedetailwidget.h \
    src/pages/favorite/favoritelistwidget.h \
    src/pages/mainwindow/mainwindow.h \
    src/pages/navigationwidget/navigationwidget.h \
    src/pages/settings/settingsdialog.h \
    src/pages/settings/settingswidget.h \
    src/pages/tag/tagdetailwidget.h \
    src/pages/tag/taglistwidget.h \
    src/pages/userdialog/userinfodialog.h \
    src/pages/utility/avataritemdelegate.h \
    src/pages/utility/avatarwidget.h \
    src/pages/utility/badgebutton.h \
    src/pages/utility/defaultportraitfactory.h \
    src/pages/utility/loadingdialog.h \
    src/tools/imageloader.h \
    src/tools/timeformatter.h \
    src/vendor/ezviewer/imageviewer_config.h \
    src/vendor/ezviewer/imageviewer_mainwindow.h \
    src/vendor/qrcode/QrCodeGenerator.h \
    src/vendor/qrcode/qrcodegen.h \
    src/wfc/client/ChatClient.h \
    src/wfc/enum/WFEnums.h \
    src/wfc/message/AddGroupMemberNotificationContent.h \
    src/wfc/message/CallStartMessageContent.h \
    src/wfc/message/ChangeGroupNameNotificationContent.h \
    src/wfc/message/ChangeGroupPortraitNotificationContent.h \
    src/wfc/message/CreateGroupNotificationContent.h \
    src/wfc/message/DismissGroupNotificationContent.h \
    src/wfc/message/FileMessageContent.h \
    src/wfc/message/GroupJoinTypeNotificationContent.h \
    src/wfc/message/GroupMuteNotificationContent.h \
    src/wfc/message/GroupPrivateChatNotificationContent.h \
    src/wfc/message/GroupSetManagerNotificationContent.h \
    src/wfc/message/ImageMessageContent.h \
    src/wfc/message/KickoffGroupMemberNotificaionContent.h \
    src/wfc/message/LocationMessageContent.h \
    src/wfc/message/MarkUnreadMessageContent.h \
    src/wfc/message/MediaMessageContent.h \
    src/wfc/message/Message.h \
    src/wfc/message/MessageContent.h \
    src/wfc/message/MessagePayload.h \
    src/wfc/message/ModifyGroupAliasNotificationContent.h \
    src/wfc/message/NotificationMessageContent.h \
    src/wfc/message/PTextMessageContent.h \
    src/wfc/message/QuitGroupNotificationContent.h \
    src/wfc/message/RecallMessageContent.h \
    src/wfc/message/RejectJoinGroupNotificationContent.h \
    src/wfc/message/SoundMessageContent.h \
    src/wfc/message/StickerMessageContent.h \
    src/wfc/message/TextMessageContent.h \
    src/wfc/message/TipNotificationContent.h \
    src/wfc/message/TransferGroupOwnerNotificationContent.h \
    src/wfc/message/TypingMessageContent.h \
    src/wfc/message/UnknownMessageContent.h \
    src/wfc/message/VideoMessageContent.h \
    src/wfc/model/ChannelInfo.h \
    src/wfc/model/ChatroomInfo.h \
    src/wfc/model/Conversation.h \
    src/wfc/model/ConversationInfo.h \
    src/wfc/model/ConversationSearchInfo.h \
    src/wfc/model/FriendRequest.h \
    src/wfc/model/GroupInfo.h \
    src/wfc/model/GroupSearchInfo.h \
    src/wfc/model/UnreadCount.h \
    src/wfc/model/UserInfo.h \
    src/wfc/model/UserSettingEntry.h \
    src/wfc/proto/include/WFClient.h \
    src/wfc/utility/JsonTools.h \
    src/wfc/utility/Serializable.h \
    src/wfc/vendor/rapidjson/allocators.h \
    src/wfc/vendor/rapidjson/cursorstreamwrapper.h \
    src/wfc/vendor/rapidjson/document.h \
    src/wfc/vendor/rapidjson/encodedstream.h \
    src/wfc/vendor/rapidjson/encodings.h \
    src/wfc/vendor/rapidjson/error/en.h \
    src/wfc/vendor/rapidjson/error/error.h \
    src/wfc/vendor/rapidjson/filereadstream.h \
    src/wfc/vendor/rapidjson/filewritestream.h \
    src/wfc/vendor/rapidjson/fwd.h \
    src/wfc/vendor/rapidjson/internal/biginteger.h \
    src/wfc/vendor/rapidjson/internal/diyfp.h \
    src/wfc/vendor/rapidjson/internal/dtoa.h \
    src/wfc/vendor/rapidjson/internal/ieee754.h \
    src/wfc/vendor/rapidjson/internal/itoa.h \
    src/wfc/vendor/rapidjson/internal/meta.h \
    src/wfc/vendor/rapidjson/internal/pow10.h \
    src/wfc/vendor/rapidjson/internal/regex.h \
    src/wfc/vendor/rapidjson/internal/stack.h \
    src/wfc/vendor/rapidjson/internal/strfunc.h \
    src/wfc/vendor/rapidjson/internal/strtod.h \
    src/wfc/vendor/rapidjson/internal/swap.h \
    src/wfc/vendor/rapidjson/istreamwrapper.h \
    src/wfc/vendor/rapidjson/memorybuffer.h \
    src/wfc/vendor/rapidjson/memorystream.h \
    src/wfc/vendor/rapidjson/msinttypes/inttypes.h \
    src/wfc/vendor/rapidjson/msinttypes/stdint.h \
    src/wfc/vendor/rapidjson/ostreamwrapper.h \
    src/wfc/vendor/rapidjson/pointer.h \
    src/wfc/vendor/rapidjson/prettywriter.h \
    src/wfc/vendor/rapidjson/rapidjson.h \
    src/wfc/vendor/rapidjson/reader.h \
    src/wfc/vendor/rapidjson/schema.h \
    src/wfc/vendor/rapidjson/stream.h \
    src/wfc/vendor/rapidjson/stringbuffer.h \
    src/wfc/vendor/rapidjson/writer.h \
    src/vendor/zenshot/commands/addcommand.h \
    src/vendor/zenshot/commands/deletecommand.h \
    src/vendor/zenshot/commands/locatorcommand.h \
    src/vendor/zenshot/commands/movecommand.h \
    src/vendor/zenshot/commands/propscommand.h \
    src/vendor/zenshot/commands/textcontentcommand.h \
    src/vendor/zenshot/config/configvalue.h \
    src/vendor/zenshot/config/xmlconfig.h \
    src/vendor/zenshot/controls/colorbutton.h \
    src/vendor/zenshot/controls/colorwidget.h \
    src/vendor/zenshot/controls/pencombobox.h \
    src/vendor/zenshot/controls/penitemwidget.h \
    src/vendor/zenshot/controls/penrenderer.h \
    src/vendor/zenshot/controls/sizebutton.h \
    src/vendor/zenshot/controls/sizeinputwidget.h \
    src/vendor/zenshot/controls/sizewidget.h \
    src/vendor/zenshot/controls/textwidget.h \
    src/vendor/zenshot/controls/textwidgetimpl.h \
    src/vendor/zenshot/core/command.h \
    src/vendor/zenshot/core/gparams.h \
    src/vendor/zenshot/core/gscale.h \
    src/vendor/zenshot/core/handle.h \
    src/vendor/zenshot/core/locator.h \
    src/vendor/zenshot/core/pointshape.h \
    src/vendor/zenshot/core/propsbar.h \
    src/vendor/zenshot/core/rectshape.h \
    src/vendor/zenshot/core/screeninfo.h \
    src/vendor/zenshot/core/screenlist.h \
    src/vendor/zenshot/core/shape.h \
    src/vendor/zenshot/core/store.h \
    src/vendor/zenshot/core/tool.h \
    src/vendor/zenshot/core/useroper.h \
    src/vendor/zenshot/core/utils.h \
    src/vendor/zenshot/handles/rectshapehandle.h \
    src/vendor/zenshot/locators/pointlocator.h \
    src/vendor/zenshot/locators/rectlocator.h \
    src/vendor/zenshot/preview/zentaobug.h \
    src/vendor/zenshot/preview/zentaopreview.h \
    src/vendor/zenshot/preview/zentaosubmit.h \
    src/vendor/zenshot/preview/zentaotips.h \
    src/vendor/zenshot/properties/arrowbar.h \
    src/vendor/zenshot/properties/curvebar.h \
    src/vendor/zenshot/properties/ellipsebar.h \
    src/vendor/zenshot/properties/linebar.h \
    src/vendor/zenshot/properties/mosaicbar.h \
    src/vendor/zenshot/properties/rectanglebar.h \
    src/vendor/zenshot/properties/textbar.h \
    src/vendor/zenshot/rapidxml/rapidxml.hpp \
    src/vendor/zenshot/rapidxml/rapidxml_iterators.hpp \
    src/vendor/zenshot/rapidxml/rapidxml_print.hpp \
    src/vendor/zenshot/rapidxml/rapidxml_utils.hpp \
    src/vendor/zenshot/screen/handletool.h \
    src/vendor/zenshot/screen/helper/screengetter.h \
    src/vendor/zenshot/screen/hovertool.h \
    src/vendor/zenshot/screen/movetool.h \
    src/vendor/zenshot/screen/shotarea.h \
    src/vendor/zenshot/screen/shotarea/areacreatetool.h \
    src/vendor/zenshot/screen/shotarea/areahandle.h \
    src/vendor/zenshot/screen/shotarea/areahandletool.h \
    src/vendor/zenshot/screen/shotarea/arealocator.h \
    src/vendor/zenshot/screen/shotarea/areamovetool.h \
    src/vendor/zenshot/screen/textassist.h \
    src/vendor/zenshot/screen/toolbar.h \
    src/vendor/zenshot/screen/workspace.h \
    src/vendor/zenshot/setting/settingdlg.h \
    src/vendor/zenshot/setting/zentaosetting.h \
    src/vendor/zenshot/setting/ztsettingdetail.h \
    src/vendor/zenshot/setting/ztsettinglist.h \
    src/vendor/zenshot/setting/ztsettinglistitem.h \
    src/vendor/zenshot/shapes/arrow.h \
    src/vendor/zenshot/shapes/curve.h \
    src/vendor/zenshot/shapes/ellipse.h \
    src/vendor/zenshot/shapes/line.h \
    src/vendor/zenshot/shapes/mosaic.h \
    src/vendor/zenshot/shapes/rectangle.h \
    src/vendor/zenshot/shapes/text.h \
    src/vendor/zenshot/spdlogwrapper.hpp \
    src/vendor/zenshot/starter.h \
    src/vendor/zenshot/starterui.h \
    src/vendor/zenshot/stores/localstore.h \
    src/vendor/zenshot/stores/memorystore.h \
    src/vendor/zenshot/tools/arrowcreatetool.h \
    src/vendor/zenshot/tools/curvecreatetool.h \
    src/vendor/zenshot/tools/ellipsecreatetool.h \
    src/vendor/zenshot/tools/linecreatetool.h \
    src/vendor/zenshot/tools/mosaiccreatetool.h \
    src/vendor/zenshot/tools/rectcreatetool.h \
    src/vendor/zenshot/tools/textcreatetool.h \
    src/vendor/zenshot/usrmetatype.h \
    src/vendor/zenshot/widget.h \
    src/vendor/zenshot/zdata/zdataitem.h \
    src/vendor/ezviewer/global.h \
    src/vendor/ezviewer/imageviewer.h \
    src/vendor/ezviewer/imagewrapper.h \
    src/vendor/ezviewer/imagefactory.h \
    src/vendor/ezviewer/picmanager.h \
    src/vendor/ezviewer/settingdialog.h \
    src/vendor/ezviewer/ui/contralbar.h \
    src/vendor/ezviewer/ui/commonsetting.h \
    src/vendor/ezviewer/ui/shortcutsetting.h \
    src/vendor/ezviewer/tools/floatframe.h \
    src/vendor/ezviewer/tools/fileassoc.h \
    src/vendor/ezviewer/tools/osrelated.h \
    src/vendor/ezviewer/tools/toolkit.h \
    src/vendor/ezviewer/tools/tooltip.h \
    src/vendor/ezviewer/tools/action.h \
    src/vendor/ezviewer/tools/actionmanager.h \
    src/vendor/ezviewer/tools/velocitytracker.h \
    src/vendor/ezviewer/3rdparty/qxmlputget.h \
    src/vendor/ezviewer/3rdparty/exif.h \
    src/vendor/ezviewer/tools/imageheader.h


TRANSLATIONS = src/vendor/zenshot/translations/chinese.ts \
               src/vendor/zenshot/translations/english.ts \
               src/vendor/ezviewer/res/EzViewer_zh_CN.ts

unix:!macx {
    message("linux VERSION")

    lessThan(QT_MAJOR_VERSION, 6): QT += x11extras

    PKGCONFIG += x11 \
            gtk+-3.0

    CONFIG += link_pkgconfig

    LIBS     += \
        -L/usr/lib/x86_64-linux-gnu/ \
            -lX11 \
            -lXext \
        -lXtst

    INCLUDEPATH += \
        /usr/include/atk-1.0/ \
        /usr/include/gdk-pixbuf-2.0/ \
        /usr/include/cairo/ \
        /usr/include/harfbuzz/ \
        /usr/include/pango-1.0/ \
        /usr/lib/x86_64-linux-gnu/glib-2.0/include/ \
        /usr/include/glib-2.0/ \
        /usr/include/gtk-3.0/

    SOURCES += \
        src/vendor/zenshot/screen/helper/windowgetter_x11.cpp \
        src/vendor/zenshot/screen/helper/screengetter_x11.cpp \
        src/vendor/zenshot/xrecord/event_monitor.cpp

    HEADERS += \
        src/vendor/zenshot/screen/helper/windowgetter.h \
        src/vendor/zenshot/xrecord/event_monitor.h
}


win32 {
    SOURCES += src/vendor/zenshot/screen/helper/windowgetter_win.cpp
    HEADERS += src/vendor/zenshot/screen/helper/windowgetter.h

    SOURCES += src/vendor/zenshot/screen/helper/screengetter_win.cpp
}

macx {
    OBJECTIVE_SOURCES += src/vendor/zenshot/screen/helper/windowgetter_mac.mm
    OBJECTIVE_HEADERS += src/vendor/zenshot/screen/helper/windowgetter.h

    SOURCES += src/vendor/zenshot/screen/helper/screengetter_mac.cpp

    LIBS += -framework Cocoa
    LIBS += -framework CoreGraphics
    LIBS += -framework CoreFoundation
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc \
    src/vendor/zenshot/main.qrc \
    src/vendor/ezviewer/res/res.qrc


INCLUDEPATH += $$PWD/src/wfc/proto
DEPENDPATH += $$PWD/src/wfc/proto

DEPENDPATH  += $$PWD/src/vendor/ezviewer $$PWD/src/vendor/ezviewer/tools $$PWD/src/vendor/ezviewer/ui $$PWD/src/vendor/ezviewer/3rdparty
INCLUDEPATH += $$PWD/src/vendor/ezviewer $$PWD/src/vendor/ezviewer/tools $$PWD/src/vendor/ezviewer/ui $$PWD/src/vendor/ezviewer/3rdparty

macx: {
    LIBS += -L$$PWD/src/wfc/proto/osx -lMarsWrapper
    EXTRA_FILES += $$PWD/src/wfc/proto/linux/sw64/libMarsWrapper.so
    QMAKE_POST_LINK += $$quote(cp $$PWD/src/wfc/proto/osx/libMarsWrapper.dylib $$OUT_PWD/)
}

unix:!macx {
    # x86_64位架构
    contains(QT_ARCH, x86_64) {
        message("ARCH: $$QT_ARCH")
        LIBS += -L$$PWD/src/wfc/proto/linux/x86_64 -lMarsWrapper
        EXTRA_FILES += $$PWD/src/wfc/proto/linux/x86_64/libMarsWrapper.so
        QMAKE_POST_LINK += $$quote(cp $$PWD/src/wfc/proto/linux/x86_64/libMarsWrapper.so $$OUT_PWD/)
    }
    # 判断是否为 ARM64 架构（兼容多种表示方式）
    contains(QMAKE_TARGET.arch, arm64|aarch64) {
        message("ARCH: $$QT_ARCH")
        LIBS += -L$$PWD/src/wfc/proto/linux/arm64 -lMarsWrapper
        EXTRA_FILES += $$PWD/src/wfc/proto/linux/arm64/libMarsWrapper.so
        QMAKE_POST_LINK += $$quote(cp $$PWD/src/wfc/proto/linux/arm64/libMarsWrapper.so $$OUT_PWD/)
    }
    # 判断是否为 LONG64 架构（兼容多种表示方式）
    contains(QMAKE_TARGET.arch, loongarch64) {
        message("ARCH: $$QT_ARCH")
        LIBS += -L$$PWD/src/wfc/proto/linux/loongarch64 -lMarsWrapper
        EXTRA_FILES += $$PWD/src/wfc/proto/linux/loongarch64/libMarsWrapper.so
        QMAKE_POST_LINK += $$quote(cp $$PWD/src/wfc/proto/linux/loongarch64/libMarsWrapper.so $$OUT_PWD/)
    }
    # 判断是否为 SW64 架构（兼容多种表示方式）
    contains(QMAKE_TARGET.arch, sw_64) {
        message("ARCH: $$QT_ARCH")
        LIBS += -L$$PWD/src/wfc/proto/linux/sw64 -lMarsWrapper
        EXTRA_FILES += $$PWD/src/wfc/proto/linux/sw64/libMarsWrapper.so
        QMAKE_POST_LINK += $$quote(cp $$PWD/src/wfc/proto/linux/sw64/libMarsWrapper.so $$OUT_PWD/)
    }
}

win32 {
    message("ARCH: $$QT_ARCH")
    # 判断是否为 64位 架构
    contains(QT_ARCH, x86_64) {
        LIBS += -L$$PWD/src/wfc/proto/windows/x64 -lMarsWrapper
        EXTRA_FILES += $$PWD/src/wfc/proto/windows/x64/MarsWrapper.dll
        QMAKE_POST_LINK += $$quote(copy /Y $$PWD/src/wfc/proto/windows/x64/MarsWrapper.dll $$OUT_PWD/)
    }
    # 判断是否为 32位 架构
    contains(QT_ARCH, x86_32) {
        LIBS += -L$$PWD/src/wfc/proto/windows/win32 -lMarsWrapper
        EXTRA_FILES += $$PWD/src/wfc/proto/windows/win32/MarsWrapper.dll
        QMAKE_POST_LINK += $$quote(copy /Y $$PWD/src/wfc/proto/windows/win32/MarsWrapper.dll $$OUT_PWD/)
    }
}


FORMS += \
    src/vendor/zenshot/setting.ui \
    src/vendor/zenshot/zentaobug.ui \
    src/vendor/zenshot/zentaodemand.ui \
    src/vendor/zenshot/zentaopreview.ui \
    src/vendor/zenshot/zentaosetting.ui \
    src/vendor/zenshot/zentaosettingdetail.ui \
    src/vendor/zenshot/zentaosettinglist.ui \
    src/vendor/zenshot/zentaosettinglistitem.ui \
    src/vendor/zenshot/zentaosubmit.ui \
    src/vendor/zenshot/zentaotips.ui \
    src/vendor/ezviewer/ui/contralbar.ui \
    src/vendor/ezviewer/ui/commonsetting.ui \
    src/vendor/ezviewer/ui/shortcutsetting.ui
