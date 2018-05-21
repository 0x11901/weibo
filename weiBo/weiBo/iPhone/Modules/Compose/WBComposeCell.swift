//
//  WBComposeCell.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/5.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

protocol WBComposeCellDelegate: NSObjectProtocol {
    func composeCell(_ composeCell: WBComposeCell, addOrChangePhotoAtIndex index: Int)
    func composeCell(_ composeCell: WBComposeCell, deletePhotoAtIndex index: Int)
}

class WBComposeCell: UICollectionViewCell {
    weak var delegate: WBComposeCellDelegate?

    var photo: UIImage? {
        didSet {
            if photo != nil {
                photoButton.setImage(photo, for: .normal)
                photoButton.isHidden = false
                addButton.isHidden = true
            } else {
                photoButton.isHidden = true
                addButton.isHidden = false
            }
        }
    }

    fileprivate lazy var addButton: UIButton = UIButton(title: nil, bgImage: "compose_pic_add", target: self, action: #selector(addOrChangePhoto))

    fileprivate lazy var photoButton: UIButton = UIButton(title: nil, target: self, action: #selector(addOrChangePhoto))

    fileprivate lazy var deleteButton: UIButton = UIButton(title: nil, image: "compose_photo_close", target: self, action: #selector(deletePhoto))

    override init(frame: CGRect) {
        super.init(frame: frame)
        setupView()
    }

    required init?(coder _: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

extension WBComposeCell {
    fileprivate func setupView() {
        contentView.addSubview(addButton)
        addButton.snp.makeConstraints { make in
            make.edges.equalTo(contentView).inset(10)
        }

        contentView.addSubview(photoButton)
        photoButton.contentMode = .scaleAspectFill
        photoButton.snp.makeConstraints { make in
            make.edges.equalTo(addButton)
        }

        photoButton.addSubview(deleteButton)
        deleteButton.sizeToFit()
        deleteButton.snp.makeConstraints { make in
            make.top.equalTo(photoButton).offset(-10)
            make.trailing.equalTo(photoButton.snp.trailing).offset(10)
        }
    }
}

extension WBComposeCell {
    @objc fileprivate func addOrChangePhoto() {
        delegate?.composeCell(self, addOrChangePhotoAtIndex: 0)
    }

    @objc fileprivate func deletePhoto() {
        delegate?.composeCell(self, deletePhotoAtIndex: 0)
    }
}
