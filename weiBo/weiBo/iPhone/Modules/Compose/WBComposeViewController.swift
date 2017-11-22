//
//  WBComposeViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.


import UIKit

private let collectionViewItemReuseIdentifier = "asdfsdfgsgsgsg"
private let maxPhoto = 9

class WBPhotoPikerLayout: UICollectionViewFlowLayout {
    private let margin: CGFloat = 10.0
    override func prepare() {
        minimumLineSpacing = margin
        minimumInteritemSpacing = margin
        let wh = (screenWidth - 4 * margin) / 3
        itemSize = CGSize(width: wh, height: wh)
    }
    
}

class WBComposeViewController: UIViewController {
    
    fileprivate lazy var leftBarButton: UIBarButtonItem = {
        let left = UIButton(title: "取消", fontSize: 15, color: .white, bgImage: "new_feature_finish_button", target: self, action: #selector(cancell))
        left.frame = CGRect(x: 0, y: 0, width: 50, height: 25)
        return UIBarButtonItem(customView: left)
    }()
    
    fileprivate lazy var rightBarButton: UIButton = {
        let right = UIButton(title: "发布", fontSize: 15, color: .white, bgImage: "new_feature_finish_button", target: self, action: #selector(compose))
        right.frame = CGRect(x: 0, y: 0, width: 50, height: 30)
        right.setTitleColor(UIColor.gray, for: .disabled)
        right.setBackgroundImage(UIImage(named: "common_button_white_disable"), for: .disabled)
        return right
    }()
    
    fileprivate lazy var titleLabel: UILabel = {
        let title = UILabel()
        let attr = NSMutableAttributedString(string: "发布微博\n", attributes: [NSAttributedStringKey.foregroundColor : UIColor.black,NSAttributedStringKey.font : UIFont.systemFont(ofSize: 16)])
        attr.append(NSAttributedString(string: "\(WBUserAccountModel.shared.screen_name!)", attributes: [NSAttributedStringKey.font: UIFont.systemFont(ofSize: 12), NSAttributedStringKey.foregroundColor: UIColor.lightGray]))
        title.attributedText = attr
        title.numberOfLines = 0
        title.textAlignment = .center
        title.sizeToFit()
        return title
    }()
    
    fileprivate lazy var toolBar: UIToolbar = {
        let tool = UIToolbar()
        let itemDictArray: [[String: Any]] =
            [["image": "compose_toolbar_picture", "Selector": #selector(emotionKeyboard)],
            ["image": "compose_trendbutton_background", "Selector": #selector(emotionKeyboard)],
            ["image": "compose_mentionbutton_background", "Selector": #selector(emotionKeyboard)],
            ["image": "compose_emoticonbutton_background", "Selector": #selector(emotionKeyboard)],
            ["image": "compose_keyboardbutton_background", "Selector": #selector(emotionKeyboard)]]
        var array:[UIBarButtonItem] = []
        for dict in itemDictArray {
            let btn = UIButton(title: "", image: dict["image"] as? String, target: self, action: dict["Selector"] as? Selector)
            let btnItem = UIBarButtonItem(customView: btn)
            btn.sizeToFit()
            array.append(btnItem)
            let flex = UIBarButtonItem(barButtonSystemItem: UIBarButtonSystemItem.flexibleSpace, target: nil, action: nil)
            array.append(flex)
        }
        array.removeLast()
        tool.items = array
        return tool
    }()
    
    fileprivate lazy var textView: WBComposeTextView = WBComposeTextView()
    
    fileprivate lazy var photoPicker: UICollectionView = {
        let cv = UICollectionView(frame: CGRect.zero, collectionViewLayout: WBPhotoPikerLayout())
        cv.dataSource = self
        cv.delegate = self
        return cv
    }()
    
    fileprivate lazy var emoji = WBEmotionKeyBoard(frame: CGRect(x: 0, y: 0, width: screenWidth, height: 216))
    
    var isShowAnima: Bool = true
    
    var isDefaultKeyboard: Bool = true
    
    var dataSource: [UIImage] = []
    
    var selectedRow: Int?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        NotificationCenter.default.addObserver(self, selector: #selector(keyboardFrameChange(sender:)), name: NSNotification.Name.UIKeyboardWillChangeFrame, object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(addOrDeleteEmotion(notification:)), name: addOrDeleteNotification, object: nil)
        setupUI()
    }
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }

}

extension WBComposeViewController {
    
    fileprivate func setupUI() {
        view.backgroundColor = UIColor.white
        setNavigation()
        setTextField()
        setToolBar()
    }
    
    fileprivate func setNavigation() {
        navigationItem.leftBarButtonItem = leftBarButton
        navigationItem.rightBarButtonItem = UIBarButtonItem(customView: rightBarButton)
        rightBarButton.isEnabled = false
        navigationItem.titleView = titleLabel
    }
    
    fileprivate func setTextField() {
        textView.delegate = self
        textView.placeHolder = "请输入..."
        textView.font = UIFont.systemFont(ofSize: 14)
        view.addSubview(textView)
        textView.snp.makeConstraints { (make) in
            make.edges.equalTo(view)
        }
        
        textView.addSubview(photoPicker)
        photoPicker.snp.makeConstraints { (make) in
            make.top.equalTo(textView).offset(100)
            make.leading.equalTo(textView).offset(10)
            make.size.equalTo(CGSize(width: screenWidth - 20, height: screenWidth - 20))
        }
        setupPicker()
    }
    
    fileprivate func setupPicker() {
        photoPicker.backgroundColor = UIColor.lightGray
        photoPicker.register(WBComposeCell.self, forCellWithReuseIdentifier: collectionViewItemReuseIdentifier)
    }
    
    fileprivate func setToolBar() {
        view.addSubview(toolBar)
        toolBar.snp.makeConstraints { (make) in
            make.leading.trailing.bottom.equalTo(view)
        }
    }
    
}

extension WBComposeViewController {
    
    @objc fileprivate func addOrDeleteEmotion(notification: Notification) {
        if let userInfo = notification.userInfo {
            //如果是删除表情
            if let isDelete = userInfo["isDelete"] as? Bool, isDelete == true {
                textView.deleteBackward()
            }
            //如果是插入表情
            if let emotion = userInfo["emotion"] as? WBEmotionModel {
                //让textView插入表情
                textView.insertEmotion(emotion: emotion)
            }
        }
    }
    
    @objc fileprivate func cancell() {
        dismiss(animated: true, completion: nil)
    }
    
    @objc fileprivate func compose() {
        let status = textView.text
        let image = dataSource.first
        NetworkTool.shared.postStatus(status: status!, image: image, success: { (response) in
            print(response!)
        }) { (err) in
            print(err)
        }
        dismiss(animated: true, completion: nil)
    }
    
    @objc fileprivate func emotionKeyboard() {
        isShowAnima = false
        textView.resignFirstResponder()
        isShowAnima = true
        if isDefaultKeyboard {
            emoji.backgroundColor = UIColor.white
            textView.inputView = emoji
            isDefaultKeyboard = false
        }else{
            textView.inputView = nil
            isDefaultKeyboard = true
        }
        textView.becomeFirstResponder()
    }
    
    @objc fileprivate func keyboardFrameChange(sender: Notification) {
        guard isShowAnima else {
            return
        }
        if let time = sender.userInfo?["UIKeyboardAnimationDurationUserInfoKey"] as? TimeInterval,
            let endY = ((sender.userInfo?["UIKeyboardFrameEndUserInfoKey"]) as? NSValue)?.cgRectValue.origin.y {
            let offset = endY - screenHeight
            toolBar.snp.updateConstraints({ (make) in
                make.bottom.equalTo(offset)
            })
            UIView.animate(withDuration: time, animations: { 
                self.view.layoutIfNeeded()
            })
        }
    }
    
}

extension WBComposeViewController: UITextViewDelegate {
    
    func textViewDidChange(_ textView: UITextView) {
        rightBarButton.isEnabled = textView.text.count > 0
    }

}

extension WBComposeViewController: UICollectionViewDataSource{
    
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int{
        return dataSource.count < maxPhoto ? dataSource.count + 1 : maxPhoto
    }

    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell{
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: collectionViewItemReuseIdentifier, for: indexPath) as! WBComposeCell
        cell.delegate = self
        if dataSource.count >= maxPhoto {
            cell.photo = dataSource[indexPath.row]
        }else{
            if indexPath.row == dataSource.count {
                cell.photo = nil
            }else{
                cell.photo = dataSource[indexPath.row]
            }
        }
        return cell
    }
    
}

extension WBComposeViewController: UICollectionViewDelegate{
    
}

extension WBComposeViewController: WBComposeCellDelegate{
    
    func composeCell(_ composeCell: WBComposeCell, addOrChangePhotoAtIndex index: Int) {
        
        guard let indexPath = photoPicker.indexPath(for: composeCell)?.row else{
            return
        }
        selectedRow = indexPath
        let picker = UIImagePickerController()
        picker.delegate = self
        present(picker, animated: true, completion: nil)
        
    }
    
    func composeCell(_ composeCell: WBComposeCell, deletePhotoAtIndex index: Int) {
        if let indexPath = photoPicker.indexPath(for: composeCell)?.row {
            dataSource.remove(at: indexPath)
        }
        photoPicker.reloadData()
    }
    
}

extension WBComposeViewController: UIImagePickerControllerDelegate,UINavigationControllerDelegate {
    
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [String : Any]) {
        
        if let originImage = info[UIImagePickerControllerOriginalImage] as? UIImage {
            OperationQueue().addOperation {
                let imageRect = CGRect(x: 0, y: 0, width: 100, height: 100)
                UIGraphicsBeginImageContext(imageRect.size)
                UIColor.clear.setFill()
                UIRectFill(imageRect)
                originImage.draw(in: imageRect)
                let wantedImage = UIGraphicsGetImageFromCurrentImageContext()
                UIGraphicsEndImageContext()
                
                if let wantedImage = wantedImage,let selectedRow = self.selectedRow {
                    if selectedRow == self.dataSource.count {
                        self.dataSource.append(wantedImage)
                    }else{
                        self.dataSource[selectedRow] = wantedImage
                    }
                }
                OperationQueue.main.addOperation {
                    self.photoPicker.reloadData()
                }
            }
            
            
            dismiss(animated: true, completion: nil)
        }
    }
    
}










