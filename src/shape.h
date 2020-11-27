Rcpp::List _findContours(Image image, int mode, int method, Rcpp::NumericVector offset) {
  std::vector< std::vector< cv::Point > > contours;
  std::vector< cv::Vec4i > hierarchy;

  cv::findContours(image.image, contours, hierarchy, mode, method, cv::Point(offset(0), offset(1)));

  int size = 0;
  for (uint i = 0; i < contours.size(); i++) {
    size += contours[i].size();
  }

  Rcpp::IntegerVector id(size);
  Rcpp::NumericVector x(size);
  Rcpp::NumericVector y(size);
  Rcpp::IntegerVector h_id(contours.size());
  Rcpp::IntegerVector after(contours.size());
  Rcpp::IntegerVector before(contours.size());
  Rcpp::IntegerVector child(contours.size());
  Rcpp::IntegerVector parent(contours.size());
  int counter = 0;
  for (uint i = 0; i < contours.size(); i++) {
    for (uint j = 0; j < contours[i].size(); j++) {
      id(counter) = i;
      x(counter) = contours[i][j].x;
      y(counter) = contours[i][j].y;
      counter += 1;
    }

    h_id(i) = i;
    after(i) = hierarchy[i][0];
    before(i) = hierarchy[i][1];
    child(i) = hierarchy[i][2];
    parent(i) = hierarchy[i][3];
  }

  Rcpp::DataFrame contours_df = Rcpp::DataFrame::create(Rcpp::Named("id") = id,
                                                        Rcpp::Named("x") = x + 1,
                                                        Rcpp::Named("y") = -y + image.nrow());
  Rcpp::DataFrame hierarchy_df = Rcpp::DataFrame::create(Rcpp::Named("id") = h_id,
                                                         Rcpp::Named("after") = after,
                                                         Rcpp::Named("before") = before,
                                                         Rcpp::Named("child") = child,
                                                         Rcpp::Named("parent") = parent);

  return Rcpp::List::create(Rcpp::Named("contours") = contours_df,
                            Rcpp::Named("hierarchy") = hierarchy_df);
}

double _contourArea(Rcpp::NumericVector x, Rcpp::NumericVector y, bool oriented) {
  std::vector<cv::Point> poly;

  for (uint i = 0; i < x.size(); i++) {
    poly.push_back(cv::Point2f(x(i), y(i)));
  }

  return cv::contourArea(poly, oriented);
}

Rcpp::List _connectedComponents(Image image, int connectivity) {
  cv::Mat labels;
  int n = cv::connectedComponents(image.image, labels, connectivity, CV_16U);

  std::vector<cv::Point> locs;
  cv::findNonZero(labels > 0, locs);

  Rcpp::NumericVector x(locs.size());
  Rcpp::NumericVector y(locs.size());
  Rcpp::NumericVector id(locs.size());

  for (uint i = 0; i < locs.size(); i++) {
    x(i) = locs[i].x + 1;
    y(i) = -locs[i].y + image.image.rows;
    id(i) = labels.at< uint16_t >(locs[i]);
  }

  return Rcpp::List::create(Rcpp::Named("n") = n - 1,
                            Rcpp::Named("labels") = Image(labels),
                            Rcpp::Named("table") = Rcpp::DataFrame::create(
                              Rcpp::Named("id") = id,
                              Rcpp::Named("x") = x,
                              Rcpp::Named("y") = y));
}

Image _watershed(Image image, Image markers) {
  cv::Mat out;
  markers.image.copyTo(out);
  cv::watershed(image.image, out);
  return Image(out);
}

Rcpp::List _fitEllipse(arma::fmat points) {
  cv::Mat cvpoints;
  arma2cv(points, cvpoints);
  cv::RotatedRect box;
  box = cv::fitEllipse(cvpoints);

  return Rcpp::List::create(Rcpp::Named("angle") = box.angle,
                            Rcpp::Named("height") = box.size.height,
                            Rcpp::Named("width") = box.size.width,
                            Rcpp::Named("center") = Rcpp::NumericVector::create(box.center.x, box.center.y));
}

Rcpp::List _fitEllipseAMS(arma::fmat points) {
  cv::Mat cvpoints;
  arma2cv(points, cvpoints);
  cv::RotatedRect box;
  box = cv::fitEllipseAMS(cvpoints);

  return Rcpp::List::create(Rcpp::Named("angle") = box.angle,
                            Rcpp::Named("height") = box.size.height,
                            Rcpp::Named("width") = box.size.width,
                            Rcpp::Named("center") = Rcpp::NumericVector::create(box.center.x, box.center.y));
}

Rcpp::List _fitEllipseDirect(arma::fmat points) {
  cv::Mat cvpoints;
  arma2cv(points, cvpoints);
  cv::RotatedRect box;
  box = cv::fitEllipseDirect(cvpoints);

  return Rcpp::List::create(Rcpp::Named("angle") = box.angle,
                            Rcpp::Named("height") = box.size.height,
                            Rcpp::Named("width") = box.size.width,
                            Rcpp::Named("center") = Rcpp::NumericVector::create(box.center.x, box.center.y));
}
